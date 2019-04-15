#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<jansson.h>

int main(int argc, char* argv[])
{

	// json_t *array1, *value;
	// json_error_t er1, er, er2;

	// array1 = json_load_file("test.json", 0, &er);

	// char *car,*car1,*car2, *car3, *car4;
	// size_t i1=22, i2=22, i3, i4, i5, i6;

	// int ress=json_unpack_ex(array1, &er1,0,"[ {s:i}, {s:i}, {s:s}]","foo", &i1, "bar", &i2, "baz", &car);
	// value= json_object_get(json_array_get(array1, 3), "array");

	// int res=json_unpack_ex(value, &er2, JSON_ALLOW_NUL, "[{s:s, s:s}, {s:s%, s?:s%}]", "id",&car1, "key", &car2,"id",&car3, &i4,"key",&car4, &i5);
	// printf("%s %s", er2.source, er2.text);
	// printf("%d, %d", res, ress);
	// printf("%d, %d", i1, i2);

	// printf("%s",json_dumps(array1, JSON_INDENT(4)));
	// json_decref(array1);

	char str[10];
	char str2[]="omar";
	char str3[]="rifai";
	const char *str4=malloc(5*sizeof(char));
	sprintf(str, "%2s%s", str2, str3);
	FILE* fp=fopen("test.json", "r");
	fscanf(fp, "%3s",str4 );

	printf("%s\n", str);
	printf("\"%s\"", str4);
	return 0;

	// GtkWidget *window;
	// gtk_init(&argc, &argv);
	// window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// gtk_widget_show(window);
	// gtk_main();
	// if(ayman()==NULL)
	// {
	// 	printf("wayre\n");
	// }
	// char filename[]="82eabcba-5e9f-11e9-8647-d663bd873d93.json";
	// const char *cmd=malloc(68*sizeof(char));
	// path_search_script;
	// printf("\nRES:%s\n",(cmd));
	// printf("\nRES:%d\n",strlen(cmd));
	// FILE* fp=popen(cmd,"r");
	// char buff[200];
	// if(fp==NULL)
	// 	return -1;

	// fscanf(fp,"%s",buff);
	// pclose(fp);
	// buff[126]='\0';
	// printf("\nRES:%d\n",strlen(buff));
	// printf("\nRES:%s\n",(buff));

	// FILE* f=fopen(buff, "r+");
	// fprintf(f, "SUCCEfrssssssssssssssssstdgfdggSS BABBYYYY");

	// fclose(f);
	// strcat(cmd, "ayre");
	// printf("\nRES:%s\n",(cmd));
	// free(cmd);
	//return 0;

}