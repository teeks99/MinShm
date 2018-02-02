
#include <Windows.h>

#define SHM_ADDRESS 0x04000000
#define SHM_SIZE_BYTES 1024
#define SHM_NAME L"TEST_NAME"

extern "C"
{
   extern struct test_other_struct {
      int a;
      int b;
      int c;
   } test_other;

   struct test_local_struct {
      int one;
      int two;
      int three;
   } test_local;
}

void AttachShm()
{
   HANDLE mapfile = CreateFileMapping(
      INVALID_HANDLE_VALUE,  // current file handle (use System page file)
      NULL,                  // default security (not inheritable)
      PAGE_READWRITE,        // read/write permission
      0,                     // size of File (high-order doubleword)
      SHM_SIZE_BYTES,        // size of File (low-order doubleword)
      SHM_NAME);             // name of mapping object

   MapViewOfFileEx(
      mapfile,               // handle to mapping object
      FILE_MAP_ALL_ACCESS,   // read/write permission
      0,                     // address offset (high-order doubleword)
      0,                     // address offset (low-order doubleword)
      SHM_SIZE_BYTES,        // size of common block
      (LPVOID)SHM_ADDRESS);  // suggested starting address
}

int main(int argc, char* argv[])
{
   AttachShm();

   // Local variable always viewable in debugger
   int test_stack = 10;

   // This works (viewable from other program in shm block) 
   // but can't be viewed in the debugger for >VS2010
   test_other.a = 30; 
   
   // Global extern "C" struct, always viewable in debugger
   test_local.one = 40;

   test_other_struct* test_other_ptr = reinterpret_cast<test_other_struct*>(SHM_ADDRESS);
   // This works and can be viewed in all VS versions
   test_other_ptr->b = 31; 

   // Put a breakpoint on the next line, view test_other.a in watch window
   return 0;
}