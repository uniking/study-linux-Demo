#ifndef MEMMAP_H  
#define MEMMAP_H  
#include <stdio.h>  
  
class MemMap  
{  
public:  
MemMap();  
~MemMap();  

bool Map(const char* szFileName);  
void UnMap();  

const    void* GetData() const { return m_pData; }  
size_t         GetSize() const { return m_uSize; }  
 
private:  
void*     m_pData;  
size_t    m_uSize;  
int       m_nFile;  
};  
#endif  


