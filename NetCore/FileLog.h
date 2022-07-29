#ifndef __FILE_LOG_H_
#define __FILE_LOG_H_

void ClearFileLog(const char* pszFileName);
void FileLog(const char* pszFileName, const char* pszLog, ...);

#endif	// #ifndef __FILE_LOG
