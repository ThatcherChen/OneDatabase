#ifndef ONEDATABASE_MAIN_H
#define ONEDATABASE_MAIN_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>


#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define TABLE_MAX_PAGES 100
#define size_of_attribute(Struct, Attribute) sizeof(((Struct *)0)->Attribute)


// Basic data type
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_NEGATIVE_ID,
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR,
    PREPARE_STRING_TOO_LONG
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

typedef struct {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef struct {
    uint32_t id;
    // '1' for null character
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
} Row;

typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;

typedef struct {
    int file_descriptor;
    uint32_t file_length;
    void *pages[TABLE_MAX_PAGES];
} Pager;

typedef struct {
    uint32_t num_rows;
    Pager *pager;
} Table;

typedef struct {
    Table *table;
    uint32_t row_num;
    bool end_of_table;
} Cursor;


extern const uint32_t ID_SIZE;
extern const uint32_t USERNAME_SIZE;
extern const uint32_t EMAIL_SIZE;
extern const uint32_t ID_OFFSET;
extern const uint32_t USERNAME_OFFSET;
extern const uint32_t EMAIL_OFFSET;
extern const uint32_t ROW_SIZE;
extern const uint32_t PAGE_SIZE;
extern const uint32_t ROWS_PER_PAGE;
extern const uint32_t TABLE_MAX_ROWS;


InputBuffer *new_input_buffer();
void print_prompt();
void print_row(Row *row);
void read_input(InputBuffer *inputBuffer);
void close_input_buffer(InputBuffer *input_buffer);
MetaCommandResult do_meta_command(InputBuffer *input_buffer, Table *table);
PrepareResult prepare_insert(InputBuffer *inputBuffer, Statement *statement);
PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement);
void serialize_row(Row *source, void *destination);
void deserialize_row(void *source, Row *destination);
ExecuteResult execute_insert(Statement *statement, Table *table);
ExecuteResult execute_select(Statement *statement, Table *table);
ExecuteResult execute_statement(Statement *statement, Table *table);
Pager *pager_open(const char *filename);
Table *db_open(const char *filename);
void db_close(Table *table);
void *get_page(Pager *pager, uint32_t page_num);
void pager_flush(Pager *pager, uint32_t page_num, uint32_t size);
Cursor *table_start(Table *table);
Cursor *table_end(Table *table);
void cursor_advance(Cursor *cursor);
void *cursor_value(Cursor *cursor);

#endif