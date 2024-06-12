#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <gtk/gtk.h>
#include <time.h>
#include <glib.h>

#include "system.h"
#include "memory_management.h"
#include "scheduling.h"
#include "file_system.h"

void print_minios(char* str);


// Functions
static void login_button_clicked(GtkWidget *widget, gpointer data);
static void process_input(GtkWidget *widget, gpointer data);
static gboolean close_window(GtkWidget *widget, GdkEvent *event, gpointer data);
static void clear_output(GtkWidget *widget, gpointer data);
int test(int a, int b);
static void write_log(const char *input, const char *user);

// Global variables
GtkWidget *login_window, *welcome_window;
GtkWidget *id_entry, *pw_entry, *input_entry, *output_text;
GHashTable *user_table;
const char *logged_user = NULL;
int fs_mode = 0;

int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);
    init_partitions();

    // Create tag for colored output
    GtkTextBuffer *buffer = NULL;
    GtkTextTag *red_tag = NULL;

    // Create user table
    user_table = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(user_table, "minios", "4321");
    g_hash_table_insert(user_table, "ssu", "1234");

    // Create login window
    login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(login_window), "Login");
    gtk_window_set_default_size(GTK_WINDOW(login_window), 300, 150);
    gtk_window_set_position(GTK_WINDOW(login_window), GTK_WIN_POS_CENTER);
    g_signal_connect(login_window, "delete-event", G_CALLBACK(close_window), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(login_window), vbox);

    GtkWidget *id_label = gtk_label_new("ID: ");
    id_entry = gtk_entry_new();
    GtkWidget *pw_label = gtk_label_new("PW: ");
    pw_entry = gtk_entry_new();
    gtk_entry_set_visibility(pw_entry, FALSE);
    // invisible?

    GtkWidget *login_button = gtk_button_new_with_label("Login");
    g_signal_connect(login_button, "clicked", G_CALLBACK(login_button_clicked), NULL);

    gtk_box_pack_start(GTK_BOX(vbox), id_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), id_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), pw_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), pw_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), login_button, FALSE, FALSE, 0);

    // Create welcome window
    welcome_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(welcome_window), "miniOS");
    gtk_window_set_default_size(GTK_WINDOW(welcome_window), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(welcome_window), GTK_WIN_POS_CENTER);
    g_signal_connect(welcome_window, "delete-event", G_CALLBACK(close_window), NULL);

    GtkWidget *welcome_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(welcome_window), welcome_vbox);

    input_entry = gtk_entry_new();
    g_signal_connect(input_entry, "activate", G_CALLBACK(process_input), NULL);

    // scroll bar
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);


    output_text = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output_text), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_window), output_text);

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_text));
    red_tag = gtk_text_buffer_create_tag(buffer, "red_tag", "foreground", "red", NULL);

    gtk_box_pack_start(GTK_BOX(welcome_vbox), input_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(welcome_vbox), scrolled_window, TRUE, TRUE, 0);

    gtk_widget_show_all(login_window);
    gtk_main();

    return 0;
}

static void login_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(id_entry));
    const gchar *pw = gtk_entry_get_text(GTK_ENTRY(pw_entry));

    const char *correct_pw = (const char *)g_hash_table_lookup(user_table, id);
    if (correct_pw != NULL && strcmp(pw, correct_pw) == 0) {
        gtk_widget_hide(login_window);
        gtk_widget_show_all(welcome_window);
        logged_user = id;
        g_print("[MiniOS SSU] Welcome, user %s\n",logged_user);
        write_log("user login", logged_user);
    } else {
        g_print("Invalid username or password! Please try again.\n");
    }
}

static void process_input(GtkWidget *widget, gpointer data) {
    const gchar *input_text = gtk_entry_get_text(GTK_ENTRY(input_entry));
    write_log(input_text, logged_user);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_text));
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_bounds(buffer, &start_iter, &end_iter);

    gtk_text_buffer_insert(buffer, &end_iter, input_text, -1);
    gtk_text_buffer_insert(buffer, &end_iter, "\n", -1);

    // basic output
    char output[200];
    int len;

    len = sprintf(output, ">>  ");

    char *token = strtok((char *)input_text, " ");

    // test
    if (token != NULL && strcmp(token, "test") == 0) {  // test
        int a, b;
        token = strtok(NULL, " ");
        if (token != NULL) {    // a
            a = atoi(token);
            token = strtok(NULL, " ");
            if (token != NULL) {    // b
                b = atoi(token);
                int result = test(a, b);

                len += sprintf(output+len, "%d\n", result);
                gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
            }
        }
    }

    // clear
    else if (token != NULL && strcmp(token, "clear") == 0) {
        clear_output(NULL, NULL);
    }
    // repeat
    else if(token != NULL && strcmp(token, "repeat") == 0) {
        token = strtok(NULL, " ");

        len += sprintf(output+len, "%s\n", token);
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    }

    // exit
    else if(token != NULL && strcmp(token, "exit") == 0) {
        print_minios("[MiniOS SSU] OS shutting down...");
        exit(0);
    }

    // memory & scheduler
    // scheduler만 cmd에 출력됨
    else if (token != NULL && strcmp(token, "print_memory") == 0) {

        char* status = print_memory_blocks_gui();
        strcpy(output, status);

        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    }
    else if (token != NULL && strcmp(token, "dyna_alloc") == 0) {

        token = strtok(NULL, " ");
        unsigned int siz = atoi(token);

        size_t size = siz;
        Process empty_process = createDefaultProcess();
        int mem_id = dyna_alloc(size, empty_process); 
        if (mem_id != -1) {
            len += sprintf(output+len, "Memory allocated and ID is %d\n", mem_id);
        } else {
            len += sprintf(output+len, "Memory allocation failed!!\n");
        }

        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    }
    else if (token != NULL && strcmp(token, "dyna_free") == 0) {

        token = strtok(NULL, " ");

        int mem_id = atoi(token);
        dyna_free(mem_id);

        len += sprintf(output+len, "memory ID %s has been successfully released.\n", token);
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    }
    else if (token != NULL && strcmp(token, "scheduling") == 0) {
        schedule();
    }


    // file system
    else if (token != NULL && strcmp(token, "filesystem") == 0 && fs_mode == 0) {
        fs_mode = 1;
        disk_init();
        fs_format();
        fs_mount();
        len += sprintf(output+len, "Entered file system mode.\n\tAvailable commands: create <name>, write <name> <content>, read <name>\n");
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    }

    else if (token != NULL && strcmp(token, "format") == 0) {
        if (fs_mode == 0) {
            len += sprintf(output+len, "file system is currently off! Please activate file system by cmd: filesystem\n");
            gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);

            return;
        }

        fs_format();
        len += sprintf(output+len, "file system formatted.\n");
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    }
    else if (token != NULL && strcmp(token, "mount") == 0) {
        if (fs_mode == 0) {
            len += sprintf(output+len, "file system is currently off! Please activate file system by cmd: filesystem\n");
            gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);

            return;
        }

        fs_mount(NULL);  // 메모리 디스크를 사용하기 때문에 경로는 필요 없음
        len += sprintf(output+len, "file system mounted.\n");
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    } 
    else if (token != NULL && strcmp(token, "create") == 0) {
        if (fs_mode == 0) {
            len += sprintf(output+len, "file system is currently off! Please activate file system by cmd: filesystem\n");
            gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);

            return;
        }

        token = strtok(NULL, " ");

        fs_create(token); 
        len += sprintf(output+len, "File '%s' created.\n", token);
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    } 
    else if (token != NULL && strcmp(token, "read") == 0) {
        if (fs_mode == 0) {
            len += sprintf(output+len, "file system is currently off! Please activate file system by cmd: filesystem\n");
            gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);

            return;
        }

        token = strtok(NULL, " ");

        char buf[BLOCK_SIZE];
        fs_read(token, buf, BLOCK_SIZE, 0);

        len += sprintf(output+len,"File content: %s\n", buf);
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    } 
    else if (token != NULL && strcmp(token, "write") == 0) {
        if (fs_mode == 0) {
            len += sprintf(output+len, "file system is currently off! Please activate file system by cmd: filesystem\n");
            gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);

            return;
        }

        // token: file name
        token = strtok(NULL, " ");
        
        char* fileName = (char*)malloc(strlen(token) + 1);
        strcpy(fileName, token);

        // token: insert context
        token = strtok(NULL, " ");

        fs_write(fileName, token, strlen(token) + 1, 0);
        
        len += sprintf(output+len, "Content '%s' written to file '%s'.\n", token, fileName);
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    } 
    else if (token != NULL && strcmp(token, "close") == 0) {
        fs_unmount();
        fs_mode = 0;

        len += sprintf(output+len, "Exit file system mode. \n");
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    }

    

    // Invalid
    else {
        sprintf(output, ">>  Invalid input!\n");
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end_iter, output, -1, "red_tag", NULL);
    }

    gtk_entry_set_text(GTK_ENTRY(input_entry), "");
}

void print_minios(char* str) {
        printf("%s\n",str);
}

// gtk functions
static gboolean close_window(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_main_quit();
    return FALSE;
}

static void clear_output(GtkWidget *widget, gpointer data) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_text));
    gtk_text_buffer_set_text(buffer, "", -1);
}


// just testing function
int test(int a, int b) {
    return a + b;
}

static void write_log(const char *input, const char *user) {
    FILE *log_file = fopen("log.txt", "a");
    if (log_file == NULL) {
        g_print("Error opening log file\n");
        return;
    }

    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y/%m/%d %T", local_time);

    if (user != NULL) {
        fprintf(log_file, "[%s] %s: %s\n", time_str, user, input);
    } else {
        fprintf(log_file, "[%s] %s\n", time_str, input);
    }

    fclose(log_file);
}