#include "Model.h"

CModel::CModel()
{
	m_filenumber = 0;
	m_threshold = 0;
}

CModel::~CModel()
{

}

int CModel::loadmodule(string& path)
{
	return 0;
}

int CModel::loadmodule()
{
	return 0;
}

int CModel::savemodule()
{
	return 0;
}

map<string, float> *CModel::getwordpro()
{
	return &m_wordPro;
}
string CModel::getmodulename()
{
	return m_modulename;
}

string CModel::getuid()
{
	return m_uid;
}

string CModel::getlevel()
{
	return m_level;
}
int CModel::getthresholdrange(float* buffer)
{
	buffer[0] = m_thresholdRange[0];
	buffer[1] = m_thresholdRange[1];

	return 0;
}
float CModel::getthreshold()
{
	return m_threshold;
}
int CModel::getfilenumber()
{
	return m_filenumber;
}
//////////////////////////////////////////////////////////////////////////
int CModel::setwordpro(map<string, float>& wordpro)
{
    map<string, float>::iterator iter = wordpro.begin();
    while(iter != wordpro.end())
    {
        m_wordPro.insert(make_pair((*iter).first, (*iter).second));
        ++iter;
    }

	return 0;
}
int CModel::setmodulename(string& modulename)
{
	m_modulename = modulename;
	return 0;
}
int CModel::setthresholdrange(float* buffer)
{
	m_thresholdRange[0] = buffer[0];
	m_thresholdRange[1] = buffer[1];
	return 0;
}
int CModel::setthreshold(float threshold)
{
	m_threshold = threshold;
	return 0;
}
int CModel::setfilenumber(int filenumber)
{
	m_filenumber = filenumber;
	return 0;
}
bool CModel::is_positive()
{
    return m_positive;
}
void CModel::set_positive(bool positive)
{
    m_positive = positive;
}
