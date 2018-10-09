#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "RSA.h"
#include "gmp.h"

extern mpz_t p, q, n, e, d;
FILE *fp, *fp_cipher, *fp_plain;

//密钥生成按钮函数
void button_KeyGenerate_callback( GtkWidget *widget, gpointer entry ) {
    fp = fopen("key_generate.txt","w+");
    key_generate(fp);
    fclose(fp);
}
//加密按钮
void button_Encrypt_callback( GtkWidget *widget, gpointer textview ) {
    gchar *plaintext;
    GtkTextIter start, end;
	// 获得文本内容
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    GtkTextIter s=start, e=end;
    //获取缓冲区明文内容
    plaintext=gtk_text_buffer_get_text(buffer, &s, &e, FALSE);
    //加密并将密文保存至ciphertext.txt
    fp_cipher = fopen("ciphertext.txt","w+");
    Encrypt(plaintext, fp_cipher);
    fclose(fp_cipher);
}
//解密按钮
void button_Decrypt_callback( GtkWidget *widget, gpointer textview ) {
    fp_cipher = fopen("ciphertext.txt","a+");
    fp_plain = fopen("plaintext.txt", "w+");
    //从ciphertext.txt获取密文解密并将明文保存至plaintext.txt
    Decrypt(fp_cipher, fp_plain);
    fclose(fp_cipher);
    fclose(fp_plain);

    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gtk_text_buffer_delete(buffer, &start, &end);
    fp_plain = fopen("plaintext.txt","r");
    gchar plain[500];
    while(!feof(fp_plain)){
        fscanf(fp_plain, "%[^\n]%*c", plain);
        strcat(plain, "\n");
        gtk_text_buffer_get_bounds(buffer, &start, &end);
        gtk_text_buffer_insert(buffer, &end, plain ,strlen(plain));
    }
    fclose(fp_plain);
}
int main (int argc, char *argv[])
{
    //初始化
    gtk_init(&argc, &argv);

    //gtk_window_new()的功能创建一个窗口并返回这个窗口的控件指针。GTK_WINDOW_TOPLEVEL指明窗口的类型为最上层的主窗口。
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_set_size_request(window, 1200, 600);				// 设置窗口的大小

    //创建表格按钮
    GtkWidget *table = gtk_table_new(5, 7, TRUE);
    gtk_container_add(GTK_CONTAINER(window), table);

    //创建按钮
    GtkWidget *button_KeyGenerate = gtk_button_new_with_label("Key Generate");
    gtk_table_attach_defaults(GTK_TABLE(table), button_KeyGenerate, 3, 4, 0, 1);

    GtkWidget *button_Encrypt = gtk_button_new_with_label("Encrypt to ciphertext.txt");
    gtk_table_attach_defaults(GTK_TABLE(table), button_Encrypt, 1, 2, 4, 5);

    GtkWidget *button_Decrypt = gtk_button_new_with_label("Decrypt from ciphertext.txt");
    gtk_table_attach_defaults(GTK_TABLE(table), button_Decrypt, 5, 6, 4, 5);


    //创建文本框
    GtkWidget *textview_encrypt = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_encrypt), TRUE);
    //gtk_editable_set_editable(GTK_EDITABLE(textview_encrypt), TRUE);
    gtk_table_attach_defaults(GTK_TABLE(table), textview_encrypt, 1, 3, 1, 3);

    GtkWidget *textview_decrypt = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_decrypt), TRUE);
    //gtk_editable_set_editable(GTK_EDITABLE(textview_decrypt), TRUE);
    gtk_table_attach_defaults(GTK_TABLE(table), textview_decrypt, 4, 6, 1, 3);

    //按钮事件
    g_signal_connect(button_KeyGenerate, "clicked", G_CALLBACK(button_KeyGenerate_callback), NULL);
    g_signal_connect(button_Encrypt, "clicked", G_CALLBACK(button_Encrypt_callback), textview_encrypt);
    g_signal_connect(button_Decrypt, "clicked", G_CALLBACK(button_Decrypt_callback), textview_decrypt);

    gtk_widget_show_all(window);

    //gtk_main()是在每个Gtk应用程序都要调用的函数。程序运行停在这里等待事件(如键盘事件或鼠标事件)的发生，等待用户来操作窗口。
    gtk_main();

    return 0;
}
