#include<stdio.h>
#include<jansson.h>
int main()
{
	size_t index;
	json_t *array1, *value, *VALUE2;
	json_error_t er;
	array1 = json_load_file("test.json", 0, &er);
	
	
	value= json_array_get(array1, 1);
	VALUE2= json_array_get(array1, 2);
	// json_decref(value);
	json_string_set(json_object_get(VALUE2, "baz"), "/zadsaddddbre" );
	json_dump_file(array1, "test.json", JSON_INDENT(4));
	json_decref(array1);
	return 0;

}