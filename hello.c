#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
  MYSQL *connection;
  MYSQL_RES *result;
  MYSQL_ROW row;
  unsigned int num_fields;
  char hostname[100];
  char port[100];
  char username[100];
  char password[100];
  char dbname[100];
  char query[1000];

  printf("Hello, MySQL World! Let's start by connecting to the database.\n\n");

  printf("Server name: ");
  fgets(hostname, sizeof(hostname), stdin);
  hostname[strlen(hostname) - 1] = '\0';
  
  printf("Server port: ");
  fgets(port, sizeof(port), stdin);
  port[strlen(port) - 1] = '\0';

  printf("User name: ");
  fgets(username, sizeof(username), stdin);
  username[strlen(username) - 1] = '\0';

  printf("Password: ");
  fgets(password, sizeof(password), stdin);
  password[strlen(password) - 1] = '\0';

  printf("Database: ");
  fgets(dbname, sizeof(dbname), stdin);
  dbname[strlen(dbname) - 1] = '\0';
  
  /* Initialize the connection object */
  connection = mysql_init(NULL);
  if (connection == NULL) {
    fprintf(stderr, "Failed to initialize: %s\n", mysql_error(connection));
    return 1;
  }

  /* Connect to the server */
  if (!mysql_real_connect(connection, hostname, username, password, dbname,
                          atoi(port), NULL, 0)) {
    fprintf(stderr, "Failed to connect to server: %s\n", mysql_error(connection));
    return 1;
  }

  printf("\nYou're now connected to the server.\n");
  
  while (true) {
    printf("\nQuery (or 'quit'): ");
    fgets(query, sizeof(query), stdin);
    query[strlen(query) - 1] = '\0';
    
    if (!strncasecmp("quit", query, sizeof(query)))
      break;

    /* Execute the query */
    if (mysql_query(connection, query)) {
      fprintf(stderr, "Error querying server: %s\n", mysql_error(connection));
      continue;
    }

    /* Get the result set */
    result = mysql_use_result(connection);
    if (result == NULL) {
      printf("No result.\n");
      continue;
    }

    /* Print all rows */
    num_fields = mysql_num_fields(result);
    while (row = mysql_fetch_row(result)) {
      unsigned long *field_lengths = mysql_fetch_lengths(result);
      if (field_lengths == NULL) {
        fprintf(stderr, "Failed to get field lengths: %s\n", mysql_error(connection));
        return 1;
      }
      for (int i = 0; i < num_fields; i++) {
        printf("[%.*s] ", (int) field_lengths[i], row[i] ? row[i] : "NULL");
      }
      printf("\n");
    }

    /* Free result set */
    mysql_free_result(result);
  }
  
  mysql_close(connection);

  return 0;
}
