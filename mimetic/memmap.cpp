/*
 * Parse and load a MIME message read from standard input and print 
 * its structure
 */
#include <iostream>
#include <mimetic/mimetic.h>
#include <mimetic/message.h>

using namespace std;
using namespace mimetic;

void printAttachment(std::string& data)
{
	Attachment att(data);
	//att.load(istr);
	if(att.isValid())
	{
		cout<<endl<<"get attachment:"<< att <<endl;
	}
	else
	{
		cout<<endl<<"it is not attachment"<<endl<<data<<endl;
	}
}

void printMimeStructure(MimeEntity* pMe, int tabcount = 0)
{
	cout<<"cout MimeEntity:"<<pMe;



	std::stringstream ssMe;
	ssMe << pMe;
	std::string MeStr = ssMe.str();
	printAttachment(MeStr);




	Header& h = pMe->header();                   // get header object
	for(int c = tabcount; c > 0; --c)            // indent nested entities
		cout << "    ";                      //
	cout << "header type:" <<h.contentType() << endl;             // prints Content-Type
	MimeEntityList& parts = pMe->body().parts(); // list of sub entities obj
	// cycle on sub entities list and print info of every item
	MimeEntityList::iterator mbit = parts.begin(), meit = parts.end();

	if(h.hasField("Content-Disposition"))
	{
		cout << "<Content-Disposition>"<<h.field("Content-Disposition").value() << "<Content-Disposition>" << endl;
	}
	if(h.hasField("Content-Transfer-Encoding"))
	{
		cout << "<Content-Transfer-Encoding>"<<h.field("Content-Transfer-Encoding").value() << "<Content-Transfer-Encoding>" << endl;
	}


	for(; mbit != meit; ++mbit)
	{
		printMimeStructure(*mbit, 1 + tabcount);
	}

	std::stringstream s;
	s << pMe->body();
	std::string bodyStr = s.str();
	cout<<"getbody:"<<bodyStr<<endl;
}

int main()
{
	ios_base::sync_with_stdio(false);        // optimization
	istreambuf_iterator<char> bit(cin), eit; // get stdin iterators
	MimeEntity me(bit,eit);                       // parse and load message
	printMimeStructure(&me);                      // print msg structure
	return 0;
}
