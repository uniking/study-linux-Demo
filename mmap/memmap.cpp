
#include "memmap.h"  
#include <unistd.h>  
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>  
  
  
MemMap::MemMap() : m_pData(0), m_uSize(0), m_nFile(0)  
{  
}  
   
MemMap::~MemMap()  
{  
    UnMap();  
}  
  
bool MemMap::Map(const char* szFileName)  
{  
    UnMap();  
    m_nFile = open(szFileName, O_RDONLY);  
    if (m_nFile < 0)   
    {   
        m_nFile = 0;  
        return false;   
    }  
  
    struct stat status;  
    fstat(m_nFile, &status);  
  
    m_uSize = status.st_size;  
    m_pData = mmap(0, m_uSize, PROT_READ, MAP_SHARED, m_nFile, 0);  
    if (MAP_FAILED != m_pData) { return true;}  
  
    close(m_nFile);  
    m_pData = NULL;  
    m_nFile = 0;  
    m_uSize = 0;  
    return false;  
 }  
  
void MemMap::UnMap()  
{  
    if(m_pData)  
    {  
        munmap(m_pData, m_uSize);  
        m_pData = NULL;  
    }  
  
    if(m_nFile)  
   {  
       close(m_nFile);  
       m_nFile = 0;  
    }  
  
    m_uSize = 0;  
}  

