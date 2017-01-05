#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <stdio.h>

xmlDocPtr  getdoc(char *docname)
{
	xmlDocPtr doc;
	//doc = xmlParseMemory(buffer, size);
	doc = xmlParseFile(docname);

	if (doc == NULL )
	{
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}

	return doc;
}

int traversal(xmlNodePtr  cur, int* level)
{
	*level = *level +1;
	//walk the tree 
	cur=cur->xmlChildrenNode;//get sub node
	while(cur !=NULL)
	{
		//char* value=NULL;
		//char* name=NULL;
		//name=(char*)(cur->name);
		//value=(char*)xmlNodeGetContent(cur);
		//xmlFree(value);

		xmlChar* att = xmlGetProp(cur, (const xmlChar*)"name");
		if(att == NULL)
			;//printf("no showname\n");
		else
		{
			int i=1;
			while(i < *level)
			{
				printf("-");
				++i;
			}
			printf("%d %s\n", *level, (char*) att);
			traversal(cur, level);
		}
		cur=cur->next; 
	}

	*level = *level-1;
}

int  getnodeset(xmlDocPtr doc, xmlChar *xpath)
{
	xmlNodePtr  cur=NULL;
	cur=xmlDocGetRootElement(doc);
	if(cur==NULL)
	{
		xmlFreeDoc(doc); 
		exit(2); 
	}

	int level = 0;
	traversal(cur, &level);

	return 0;
}





int main(int argc, char **argv)
{
	char *docname;
	xmlDocPtr doc;
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
	getnodeset (doc, xpath);


	xmlFreeDoc(doc);
	xmlCleanupParser();
	return (1);
}
