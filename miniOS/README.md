# miniOS

miniOS-project/

├── boot/                   # 부트로더 소스 코드  

O   └── boot.asm            # 부트로더 어셈블리 코드, miniOS 기능을 화면에 출력  

O   └── Makefile            # 부팅 시 출력되는 화면을 확인하기 위한 메이크파일

├── include/                # 헤더 파일  

O   └── file_system.h       # 파일 시스템 헤더 
 
O   └── memory_management.h # 메모리 관리 헤더 파일 

O   └── scheduling.h        # 스케줄링 헤더 파일

O   └── system.h            # 시스템 헤더 파일

├── kernel/                 # 커널 소스 코드  

O   └── modules             # 커널 모듈  
  
   O   └── file_system.c #파일 시스템 소스 코드 파일
   
   O   └── file_system.o #파일 시스템 오브젝트 파일
   
   O   └── memory_management.c #메모리 관리 소스 코드 파일
   
   O   └── memory_management.o #메모리 관리 오브젝트 파일
   
   O   └── sheduling.c # 프로세스 스케줄링 소스 코드 파일
   
   O   └── sheduling.o # 프로세스 스케줄링 오브젝트 파일

O   └── kernel.c            # 커널 소스 파일

O   └── kernel.o            # 커널 오브젝트 파일

O   └── process.c           # 프로세스 소스 파일

O   └── system.c            # 시스템 소스 파일

O   └── system.o            # 시스템 오브젝트 파일

├── Makefile                # 전체 프로젝트 빌드 자동화를 위한 메이크파일  

├── README.md               # 프로젝트 내 디렉토리 및 폴더 설명

├── minios                  # minios

README.md               # 프로젝트 실행 방법 설명
