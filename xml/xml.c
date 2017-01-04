#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <stdio.h>

xmlDocPtr  getdoc(char *docname)
{
	xmlDocPtr doc;
	doc = xmlParseFile(docname);

	if (doc == NULL )
	{
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}

	return doc;
}

xmlXPathObjectPtr  getnodeset(xmlDocPtr doc, xmlChar *xpath)
{
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	if (context == NULL)
	{
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL)
	{
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	if(xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
		printf("No result\n");
		return NULL;
	}
	return result;
}





int main(int argc, char **argv)
{
	char *docname;
	xmlDocPtr doc;
	/*
	// all node info
	/ node path
	* every node
	@ attribute
	[@att] node that have att, note that get node value not att.
	*/
	//xmlChar *xpath = (xmlChar*) "//keyword";
	//xmlChar *xpath = (xmlChar*) "//proto";
	//xmlChar *xpath = (xmlChar*) "//proto//@showname";
	//xmlChar *xpath = (xmlChar*) "//proto[@showname]";
	//xmlChar *xpath = (xmlChar*) "//proto[@name=\"ip\"]//@showname";
	//xmlChar *xpath = (xmlChar*) "//proto[@name=\"ip\"]/@showname";
	xmlChar *xpath = (xmlChar*) "/packet/proto[@name=\"ip\"]/@showname"; // packet节点下， 属性name等于ip的proto节点之下，属性showname的值
	xmlNodeSetPtr nodeset;
	xmlXPathObjectPtr result;
	int i;
	xmlChar *keyword;

	if (argc <= 1)
	{
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}

	docname = argv[1];
	doc = getdoc(docname);
	result = getnodeset (doc, xpath);
	if (result)
	{
		nodeset = result->nodesetval;
		for (i=0; i < nodeset->nodeNr; i++)
		{
			keyword = xmlNodeListGetString(doc, nodeset->nodeTab[i]->xmlChildrenNode, 1);
			printf("keyword: %s\n", keyword);
			xmlFree(keyword);
		}
		xmlXPathFreeObject(result);
	}
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return (1);
}
