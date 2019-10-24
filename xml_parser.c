#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

enum Error {
	ER_XMLR = -1,
	ER_XMLEMPTY = -2,
};

xmlXPathObject *get_nodeset(xmlDoc *doc, const char *xpath)
{
	xmlXPathContext *context = NULL;
	xmlXPathObject *result = NULL;

	context = xmlXPathNewContext(doc);

	if(context == NULL) {
		fprintf(stderr, "error in context\n");
		return NULL;
	}

	result = xmlXPathEvalExpression(BAD_CAST xpath, context);

	xmlXPathFreeContext(context);

	if(result == NULL) {
		xmlXPathFreeObject(result);
		printf("no result\n");
		return NULL;
	}

	return result;
}

int read_xml_file(xmlDoc **xml_doc, const char *path)
{
	(*xml_doc) = xmlParseFile(path);

	if((*xml_doc) == NULL) {
		fprintf(stderr, "unable to open xml doc\n");
		return ER_XMLR;
	}

	return 0;

}

/* evaluate xpath expression and store result in result array */
int eval_xpath_expr(xmlDoc *xml_doc, const char *xpath_expr, char ***result,int *result_count)
{
	xmlXPathContext *xpath_context = NULL;
	xmlXPathObject *xpath_result = NULL;
	xmlNodeSet *nodeset = NULL;
	xmlChar *tmp = NULL;
	int tmp_count = 0;
	xpath_context = xmlXPathNewContext(xml_doc);

	xpath_result = get_nodeset(xml_doc, xpath_expr);

	if(xpath_result) {
		nodeset = xpath_result->nodesetval;

	}
	
	if(nodeset->nodeNr == 0) {
		xmlXPathFreeContext(xpath_context);
		xmlXPathFreeObject(xpath_result);
		xmlFreeDoc(xml_doc);
		return ER_XMLEMPTY;
	}
	
	*result = calloc(nodeset->nodeNr, sizeof(char *));

	if(*result == NULL) {
		fprintf(stderr, "error alloc\n");
		return -1;
	}

	for(int i = 0; i < nodeset->nodeNr; i++) {
		tmp = xmlNodeListGetString(xml_doc, nodeset->nodeTab[i]->xmlChildrenNode, 1);
		(*result)[i] = calloc(strlen((char *)tmp) + 1, sizeof(char));

		if((*result)[i] == NULL) {
			fprintf(stderr, "error alloc\n");
			return -1;
		}
		memcpy((*result)[i], tmp, strlen((char *)tmp));
		
		tmp_count = i;
		xmlFree(tmp);
	}

	(*result_count) = tmp_count;
	xmlXPathFreeContext(xpath_context);
	xmlXPathFreeObject(xpath_result);

	return 0;

}

int main(void)
{
	xmlDoc *xml_doc = NULL;
	xml_doc = NULL;
	char **result = NULL;
	int count = 0;
	int status = 0;
	read_xml_file(&xml_doc, "test.xml");

	status = eval_xpath_expr(xml_doc, "//child/cvalue", &result, &count);

	if(status != 0) {
		fprintf(stderr, "no results\n");
		return status;
	}

	xmlFreeDoc(xml_doc);

	for(int i = 0; i <= count; i++) {
		printf("#%s\n", result[i]);
		free(result[i]);
	}

	free(result);

	return 0;
}
