# ActionSquare_Task5
## 프로젝트 소개
1. 이 프로젝트는 ASCII 테트리스다.
2. 이 프로젝트는 클래식 테트리스의 규칙과 로직을 바탕으로 구현되었다.
## Game Manual
1. 레벨에 따라 속도가 빨라지고 최대한 높은 점수를 획득하는 것을 목적으로 플레이된다.
2. 조작키는 ←, →, ↑, ↓, space다.
- ←, → : 조작하고 있는 블록 좌우 이동
- ↑ : 조작하고 있는 블록 시계방향 회전
- ↓ : 소프트 드롭
- space : 일시정지
## Game Logic
1. 블록의 회전은 테트리스 게임들이 사용하고 있는 SRS(Super Rotation System)로직의 테이블을 사용하여 구현하였다. 그러므로 실제로 사용되는 T스핀 같은 테트리스 고급 기술도 사용 가능하다.
2. 점수 계산은 T스핀에 대한 추가 점수를 제외한 기본적인 테트리스의 점수 계산만 적용했다. 한 번에 제거하는 라인과 레벨에 따라 점수가 부여된다.
## 구현해야할 기능
1. 인트로 화면, 플레이 화면, 게임오버 화면
State에 따라 구현 및 출력
(난이도 : 2)
2. 점수를 텍스트 파일에 저장해서 프로그램
켤 때 불러오고 게임오버 시 점수와 이름 추가
(난이도 : 3)
3. 테트리스 게임이 완성되었으면 서버, 클라
사용해서 멀티 테트리스 구현해보기
(난이도: 10000000000)

