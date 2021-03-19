### AccessExcel
> _OLE automation을 활용한 엑셀 컨트롤, 엑셀의 옵션 설정값 취득_

### MultiLangFramework
> _다국어 프로그램 지원 프레임워크_
* 리소스의 string table 대신 JSON 형식의 string table에 언어별 리소스를 저장함으로써 배열 등을 표현하기에 용이함, JSON을 사용함으로써 이기종 간의 호환 가능
* 하나의 리소스 화면에서 다국어를 표현하는 방법이 아닌, 언어별 리소스 DLL에 연결하는 방식을 사용함으로써 언어별로 각기 다른 리소스 및 화면 구성에 용이
* 빌드할 때마다 자동으로 버전 정보를 올려주도록 구성했기 때문에 버전별 배포 시에 용이
* 윈도우 레지스트리에 등록 및 삭제 기능 추가
