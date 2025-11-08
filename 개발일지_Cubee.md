# 화재 시뮬레이션 프로젝트 개발일지

**프로젝트명**: firepjt_first (화재 대피 시뮬레이션)
**담당자**: cubee021
**담당 파트**: 멀티플레이어 시스템 (Lobby/House Game Mode), UI 시스템, NPC 시스템
**개발 브랜치**: Cubee
**개발 기간**: 2025.10.15 ~ 2025.10.31

---

## 📋 목차

1. [프로젝트 개요](#프로젝트-개요)
2. [담당 구현 내역](#담당-구현-내역)
3. [일자별 개발 로그](#일자별-개발-로그)
4. [주요 기술 스택](#주요-기술-스택)
5. [최종 성과](#최종-성과)

---

## 🎯 프로젝트 개요

### 프로젝트 소개
언리얼 엔진 5.6 기반의 **멀티플레이어 화재 대피 시뮬레이션 게임**입니다. 2인 협동 플레이로 소방관과 시민 역할을 선택하여 화재 상황에서 미션을 수행합니다.

### 담당 역할
- **멀티플레이어 네트워크 시스템** 구축 (Lobby → Game 흐름)
- **게임 진행 시스템** 설계 및 구현 (GameMode, GameState, PlayerState)
- **역할별 UI 시스템** 구현 (InGame, Lobby, Victory, GameOver 위젯)
- **NPC 대화 시스템** 구현 (DataTable 기반 대화형 NPC)

---

## 🛠 담당 구현 내역

### 핵심 시스템

#### 1. **멀티플레이어 로비 시스템**
**파일 위치**: `Source/firepjt_first/*/Cubee/`

**구현 클래스**:
- `ALobbyGameMode` - 로비 게임 모드
  - 플레이어 접속 관리 (`PostLogin`, `Logout`)
  - 최대 2명 제한 시스템
  - 게임 시작 시 ServerTravel로 HouseMap 이동

- `ULobbyWidget` - 로비 UI 위젯
  - 역할 선택 버튼 (소방관/시민)
  - 플레이어 수 표시 (Current/Max)
  - 게임 시작 버튼

- `UMainWidget` - 메인 메뉴 UI
  - Host/Join 버튼
  - 서버 생성 및 참가 기능

- `UJoinWidget` - 서버 참가 UI
  - IP 주소 입력 기능
  - 서버 연결 로직

#### 2. **House 게임 진행 시스템**
**파일 위치**: `Source/firepjt_first/Public/Cubee/HouseGameMode.h`, `HouseGameState.h`, `HousePlayerState.h`

**구현 클래스**:
- `AHouseGameMode` - 메인 게임 모드
  - **Timer 기반 미션 관리** (FTimerHandle 사용, Tick 비활성화)
  - **Phase 시스템** (WaitingToStart → Intro → GameStart → MissionStart → MissionComplete → Victory/GameOver)
  - **역할별 Pawn 생성** (`GetDefaultPawnClassForController_Implementation`)
  - 미션 진행 로직 (`StartMission`, `CheckMissionProgress`, `AdvanceToNextMission`)
  - 미션 실패/성공 처리 (`FailMission`, `Victory`)

- `AHouseGameState` - 게임 상태 (Replicated)
  - 현재 Phase 복제 (`OnRep_CurrentPhase`)
  - 미션 타이머 정보 (`MissionTimeRemaining`, `MissionTimeLimit`)
  - 현재 미션 인덱스 (`CurrentMissionIndex`)
  - Phase 변경 델리게이트 (`FOnPhaseChanged`)

- `AHousePlayerState` - 플레이어 상태 (Replicated)
  - 역할 정보 (`EPlayerRole`: Firefighter/Citizen)
  - 미션 완료 여부 (`bIsMissionComplete`)
  - GameMode로 미션 완료 보고 (`SetMissionComplete`)

#### 3. **플레이어 컨트롤러 통합**
**파일 위치**: `Source/firepjt_first/firepjt_firstPlayerController.h/cpp`

**주요 기능**:
- **맵별 Input Mode 자동 설정**
  - Lobby Map: `FInputModeUIOnly` (마우스 표시)
  - House Map: `FInputModeGameOnly` (마우스 숨김)

- **GameState 이벤트 바인딩**
  - Phase 변경 시 UI 자동 생성/파괴
  - `OnGamePhaseChanged()`: Phase별 위젯 관리

- **UI 위젯 생성 시스템**
  - Intro: 시네마틱 재생
  - GameStart: InGameWidget 생성 및 MissionDataTable 할당
  - MissionStart: 미션 텍스트 UI 업데이트
  - GameOver/Victory: 결과 UI 표시

- **시네마틱 제어**
  - Server RPC를 통한 시네마틱 재생 (`Server_PlayMissionOneCinematic`, etc.)
  - 스킵 기능 (`Server_CinematicSkip`)

- **NPC 대화 시스템 연동**
  - `Server_StartDialogue()`, `Server_ProgressDialogue()`

#### 4. **InGame UI 시스템**
**파일 위치**: `Source/firepjt_first/Public/Cubee/InGameWidget.h/cpp`

**구현 기능**:
- **DataTable 기반 미션 시스템**
  - `SetMissionDataTable()`: 역할별 미션 데이터 할당
  - `SetMissionTextFromIndex()`: 미션 텍스트 UI 업데이트
  - `FindRow<FMissionTable>`를 사용한 DataTable Row 검색

- **실시간 타이머 표시**
  - `NativeTick`에서 GameState 타이머 연동
  - MM:SS 포맷으로 남은 시간 표시

- **미션 정보 표시**
  - 미션 ID, 제목, 설명 TextBlock 바인딩

#### 5. **NPC 대화 시스템**
**파일 위치**: `Source/firepjt_first/Public/Cubee/NPC/`

**구현 클래스**:
- `ANPCBase` - NPC 베이스 클래스
  - DataTable 기반 대화 시스템
  - Multicast RPC를 통한 대화 동기화
  - `StartDialogue()`, `ProgressDialogue()` 함수

- `AJinsang` - 진상 NPC
  - NPCBase 상속
  - Sitting_Angry 애니메이션 연동
  - 대화 상호작용 기능

- `UJinsangAnim` - 진상 애니메이션 블루프린트
  - 애니몽타주 재생 (AM_Angry)

- `UNPCWidget` - NPC UI 위젯
  - 대화 텍스트 표시
  - 선택지 버튼 시스템

#### 6. **역할 시스템**
**파일 위치**: `Source/firepjt_first/Public/Cubee/HousePlayerState.h`

**Enum 정의**:
```cpp
UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
    None        UMETA(DisplayName = "None"),
    Firefighter UMETA(DisplayName = "Firefighter"),
    Citizen     UMETA(DisplayName = "Citizen")
};
```

**구현 기능**:
- FireGameInstance에 역할 정보 저장
- HouseGameMode에서 역할에 따른 Pawn 생성
- 역할별 MissionDataTable 분리 (DT_Fireman, DT_People)

#### 7. **기타 UI 위젯**
**파일 위치**: `Source/firepjt_first/Public/Cubee/`

- `UVictoryWidget` - 승리 화면 UI
- `UGameOverWidget` - 게임오버 화면 UI
- `UInfoWidget` - 정보 표시 위젯 (DataAsset 기반)

---

## 📅 일자별 개발 로그

### 2025.10.15 (화)
**작업 내용**: HouseGameMode 기본 구조 구축

#### 구현 사항
1. **HouseGameMode.cpp 전체 구현 완료**
   - Constructor: GameStateClass(`AHouseGameState`), PlayerStateClass(`AHousePlayerState`) 설정
   - BeginPlay: 초기 Phase를 `WaitingToStart`로 설정
   - **Tick 비활성화**: 성능 최적화를 위해 Timer 기반으로 변경

2. **Timer 기반 미션 시스템 구현**
   - `FTimerHandle MissionTimerHandle` 사용
   - `UpdateMissionTimer()`: 1초마다 호출되어 시간 감소 및 실패 체크
   - 30초/10초 경고 로그 출력 기능

3. **게임 진행 로직 구현**
   - `StartGame()`: Cinematic Phase → 첫 번째 미션 시작
   - `StartMission()`: 미션 인덱스 설정, 모든 플레이어 상태 리셋
   - `ReportMissionComplete()`: 플레이어별 미션 완료 보고
   - `CheckMissionProgress()`: 모든 플레이어 완료 시 다음 미션으로 전환

4. **미션 완료/실패 처리**
   - `AdvanceToNextMission()`: 다음 미션 시작 또는 Victory 처리
   - `FailMission()`: GameOver Phase 전환, 타이머 중지

#### 빌드 결과
✅ 컴파일 에러 수정 (특수문자 제거) 후 정상 빌드 완료

#### Commit
```
[feat] Implement HouseGameMode with timer-based mission system
- Timer 기반 미션 관리 시스템
- Phase 전환 시스템 (WaitingToStart → Victory/GameOver)
- 멀티플레이어 미션 진행 체크
```

---

### 2025.10.16 (수)
**작업 내용**: InGameWidget DataTable 연동 및 역할 시스템 설계

#### 구현 사항
1. **팀원 코드 분석 및 통합**
   - main 브랜치 merge 내역 확인 (commit: 8efc2bc)
   - `PeopleOnePC` (팀원 작업) 상속 구조 파악
   - InGameWidget 표시 문제 원인 분석 (GamePhase 이벤트 기반)

2. **DataTable 기반 미션 시스템 구현**
   - `InGameWidget.h`: `MissionDataTable` 참조 변수 추가
   - `SetMissionTextFromIndex()` 함수 구현:
     ```cpp
     FMissionTable* Row = MissionDataTable->FindRow<FMissionTable>(RowName, TEXT(""));
     ```
   - mission_head, mission_description을 UI TextBlock에 바인딩
   - Txt_Id 텍스트 포맷팅 버그 수정 (FString::Printf 사용)

3. **PlayerController에서 DataTable 자동 할당**
   - `firepjt_firstPlayerController.h`: `MissionDataTable` UPROPERTY 추가
   - `OnGamePhaseChanged()`: InGameWidget 생성 시 DataTable 할당

4. **멀티플레이어 역할 시스템 설계 논의**
   - 하나의 GameMode에서 여러 PlayerController 사용 가능 확인
   - 역할별 구현 방법 3가지 비교:
     1. PlayerController별 DataTable 설정 (간단)
     2. **PlayerState 기반 역할 관리** (멀티플레이 권장) ← 채택
     3. GameMode PostLogin 오버라이드 (중앙화)

#### Commit
```
[feat] InGameWidget에 DataTable 기반 미션 시스템 구현
- InGameWidget DataTable 연동 및 SetMissionTextFromIndex() 구현
- PlayerController에서 역할별 MissionDataTable 자동 할당
```

---

### 2025.10.17 (목)
**작업 내용**: 미션 진행 시스템 버그 수정 및 테스트

#### 구현 사항
1. **InGameWidget DataTable 연동 문제 해결**
   - 문제: Blueprint에서 DataTable 할당 불가
   - 해결: `EditDefaultsOnly` → `EditAnywhere, BlueprintReadWrite`로 변경
   - 위치: `firepjt_firstPlayerController.h:79-83`

2. **미션 진행 흐름 구축**
   - PlayerState → GameMode 미션 완료 보고 구조 설계
   - `HousePlayerState::SetMissionComplete()`에서 GameMode 호출 추가
   - 네트워크 동기화: `HasAuthority()` 체크로 서버 전용 실행

3. **순환 호출 버그 수정** 🐛
   - 문제: `SetMissionComplete()` ↔ `ReportMissionComplete()` 무한 반복 크래시
   - 해결: `ReportMissionComplete()`에서 중복 `SetMissionComplete()` 호출 제거
   - 위치: `HouseGameMode.cpp:86-96`

4. **멀티플레이어 미션 진행 테스트 성공** ✅
   - 두 플레이어 모두 미션 완료 시 다음 미션으로 자동 진행
   - 미션 1 → 2 → 3 정상 동작 확인

5. **Blueprint 설정 오류 해결**
   - 문제: BP_HouseGameMode의 Player State Class가 None으로 초기화됨
   - 해결: Blueprint에서 `HousePlayerState`로 재설정

#### Commit
```
[fix] Implement mission progression system with multiplayer support
- 순환 호출 버그 수정 및 네트워크 동기화 처리
- 미션 1→2→3 순차 진행 기능 완성 및 테스트 완료
```

---

### 2025.10.20 (일)
**작업 내용**: 프로토타입 최종 완성

#### 구현 사항
1. **ASequencePlayer 기능 개선**
   - 시네마틱 재생 관련 기능 추가
   - 시네마틱 연출 시스템 강화

2. **HouseGameMode 및 PlayerState 수정**
   - HouseGameMode.cpp 로직 리팩토링 (10줄 변경)
   - HousePlayerState.h 미세 조정

3. **PlayerController 확장**
   - firepjt_firstPlayerController에 새 기능 20줄 추가
   - 시네마틱 제어 함수 4개 추가:
     - `Server_PlayMissionOneCinematic()`
     - `Server_PlayMissionTwoCinematic()`
     - `Server_PlayMissionThreeCinematic()`
     - `Server_CinematicSkip()`

4. **Victory UI 최종 보완**
   - WBP_Victory 위젯 최종 수정 및 안정화

5. **미션 시스템 완성**
   - Calling Sound 추가
   - 미션 간 전환 로직 구현
   - 미션 One 시네마틱 진행 연결

6. **맵 업데이트**
   - HouseMap.umap 업데이트 (씬 구성 변경)

#### Commit
```
[feat] 20251020 프로토타입 최종
- 시퀀스 플레이어 시스템 개선
- Victory UI 완성
- 미션 시스템 통합 완료
```

---

### 2025.10.23 (수)
**작업 내용**: 역할별 스폰 시스템 구현

#### 구현 사항
1. **역할별 PlayerStart 분리 시스템 구현**
   - `AHouseGameMode::ChoosePlayerStart_Implementation()` 오버라이드
   - FireGameInstance에서 플레이어 역할 조회
   - 역할에 따라 적절한 PlayerStart Tag 검색 (Firefighter/Citizen)

2. **FindPlayerStartWithTag() 헬퍼 함수 추가**
   ```cpp
   AActor* FindPlayerStartWithTag(const FString& Tag);
   ```
   - TActorIterator로 모든 PlayerStart 순회
   - PlayerStartTag가 일치하는 액터 반환

3. **레벨 에디터 설정**
   - HouseMap에 PlayerStart 2개 배치
   - Player Start Tag 설정 (Firefighter, Citizen)

#### 발견된 문제 🐛
- 서버 플레이어: 항상 CitizenStart에서 스폰
- 클라이언트 플레이어: FiremanStart에서 스폰
- 역할 선택과 스폰 위치가 반대로 적용됨

#### 원인 분석
- FireGameInstance에서 PlayerName/PlayerId를 Key로 사용
- ServerTravel 시 PlayerState 동기화 타이밍 이슈
- ChoosePlayerStart 호출 시점에 역할 정보 조회 실패

#### 해결 방향
- NetPlayerIndex 기반 Key 사용으로 변경 제안
- 디버그 로그 추가하여 정확한 원인 파악 필요

#### Commit
```
[feat] 역할별 PlayerStart 분리 구현 (스폰 순서 이슈 미해결)
- Tag 기반 PlayerStart 선택 로직 설계
- FireGameInstance Key 방식 개선 필요
```

---

### 2025.10.24 (목)
**작업 내용**: 멀티플레이 시스템 개발 진행

#### 구현 사항
1. **소방관/시민 역할 선택 로직 구현**
   - Lobby에서 역할 선택 버튼 작동 확인
   - FireGameInstance에 역할 정보 저장

2. **역할별 차별화된 게임플레이 메커니즘 설계**
   - 소방관/시민 미션 분리 (DT_Fireman, DT_People)
   - 역할별 Pawn Class 설정 가능

3. **HouseMap, HouseGameMode, PlayerController 수정**
   - 맵 수정 사항 확인
   - 게임모드 변경 사항 파악
   - 컨트롤러 수정 내용 확인

4. **위젯 수정 사항 확인 및 개선**
   - Skip 버튼 동기화 관련 이슈 점검

---

### 2025.10.29 (화)
**작업 내용**: NPC 시스템 검토 및 구현 준비

#### 구현 사항
1. **NPC 시스템 파일 구조 확인**
   - `Source/firepjt_first/Private/Cubee/NPC/` 확인
   - `Source/firepjt_first/Public/Cubee/NPC/` 헤더 확인
   - `Content/CustomContents/NPC/` 콘텐츠 에셋 확인

2. **NPC 기능과 로비 시스템 통합 검토**
   - NPCBase, Jinsang, FireZombie 클래스 분석
   - 멀티플레이 환경에서 NPC 상호작용 설계

---

### 2025.10.30 (수)
**작업 내용**: NPC 시스템 구현

#### 구현 사항
1. **NPCBase 클래스 구현**
   - DataTable 기반 대화 시스템 설계
   - Multicast RPC를 통한 네트워크 동기화
   - `StartDialogue()`, `ProgressDialogue()` 함수 구현

2. **Jinsang NPC 구현**
   - NPCBase 상속
   - 대화 상호작용 기능 추가

3. **NPCWidget UI 연동**
   - 대화 텍스트 표시 위젯
   - 선택지 버튼 시스템

4. **멀티플레이 환경에서 NPC 동작 검증**
   - Replication 설정 확인
   - 클라이언트-서버 간 NPC 상태 동기화 테스트

---

### 2025.10.31 (목)
**작업 내용**: NPC 대화 시스템 및 애니메이션 통합

#### 구현 사항
1. **NPC 대화 시스템 완성**
   - NPCBase, NPCWidget 변경사항 완료
   - DT_Jinsang DataTable 작성
   - 대화형 NPC 기능 테스트 완료

2. **애니메이션 통합**
   - ABP_Jinsang 애니메이션 블루프린트 설정
   - Sitting_Angry 애니메이션 재생 테스트
   - AM_Angry 애니몽타주 연결

3. **PlayerController와 NPC 상호작용 연동**
   - `Server_StartDialogue()` 구현
   - `Server_ProgressDialogue(int32 NextID)` 구현
   - RPC를 통한 대화 진행 동기화

4. **HouseMap 통합 테스트**
   - NPC 배치 확인
   - 멀티플레이 환경에서 대화 동작 검증

---

## 🔧 주요 기술 스택

### 언리얼 엔진 시스템
- **Network System**: Server RPC, Client RPC, Multicast RPC
- **Replication**: Replicated Properties, OnRep Notify
- **Game Framework**: GameMode, GameState, PlayerState, PlayerController
- **Input System**: Enhanced Input System (UE5)
- **UI System**: UMG Widget, DataBinding
- **Timer System**: FTimerHandle, FTimerManager

### 디자인 패턴
- **MVC 패턴**: GameMode(Controller) - GameState(Model) - Widget(View)
- **Observer 패턴**: Delegate를 통한 Phase 변경 알림
- **State 패턴**: EGamePhase를 통한 게임 상태 관리
- **Component 패턴**: Actor Component 기반 모듈화

### 최적화 기법
- **Timer 기반 업데이트**: Tick 대신 FTimerHandle 사용
- **Network Optimization**: HasAuthority() 체크로 서버 전용 실행
- **UI Optimization**: NativeTick에서 필요한 경우만 업데이트

---

## 🎖 최종 성과

### 구현 완료 항목
✅ **멀티플레이어 로비 시스템** (최대 2인, 역할 선택)
✅ **House 게임 진행 시스템** (Phase 기반 미션 관리)
✅ **역할별 UI 시스템** (InGame, Lobby, Victory, GameOver)
✅ **DataTable 기반 미션 시스템** (역할별 미션 분리)
✅ **NPC 대화 시스템** (DataTable 기반, 네트워크 동기화)
✅ **시네마틱 제어 시스템** (RPC 기반 시퀀스 재생)
✅ **Timer 기반 미션 타이머** (실시간 UI 업데이트)

### 핵심 파일 목록
**C++ 클래스** (Cubee 폴더):
- `ALobbyGameMode` - 로비 게임 모드
- `AHouseGameMode` - 메인 게임 모드
- `AHouseGameState` - 게임 상태 (Replicated)
- `AHousePlayerState` - 플레이어 상태 (Replicated)
- `ULobbyWidget` - 로비 UI
- `UMainWidget` - 메인 메뉴 UI
- `UJoinWidget` - 서버 참가 UI
- `UInGameWidget` - 인게임 UI
- `UVictoryWidget` - 승리 UI
- `UGameOverWidget` - 게임오버 UI
- `UInfoWidget` - 정보 표시 UI
- `ANPCBase` - NPC 베이스 클래스
- `AJinsang` - 진상 NPC
- `UNPCWidget` - NPC 대화 UI
- `UFireGameInstance` - 역할 정보 저장

**PlayerController**:
- `Afirepjt_firstPlayerController` - 베이스 플레이어 컨트롤러
  - 맵별 Input Mode 자동 설정
  - GameState 이벤트 바인딩
  - UI 위젯 생성 시스템
  - 시네마틱 제어 RPC
  - NPC 대화 RPC

### 네트워크 아키텍처
```
[Client] LobbyWidget → ALobbyGameMode (Server) → ServerTravel
         ↓
[Server] AHouseGameMode
         ├─ AHouseGameState (Replicated)
         │  ├─ CurrentPhase (OnRep)
         │  ├─ MissionTimeRemaining
         │  └─ CurrentMissionIndex
         └─ AHousePlayerState (Replicated)
            ├─ PlayerRole (Firefighter/Citizen)
            └─ bIsMissionComplete
         ↓
[Client] InGameWidget (NativeTick)
         └─ UI 자동 업데이트
```

### 기술적 성과
- **완전한 멀티플레이어 네트워크 동기화** (Server-Client RPC)
- **Tick 대신 Timer 사용**으로 성능 최적화
- **DataTable 기반 데이터 관리**로 유지보수성 향상
- **Phase 기반 게임 흐름 관리**로 확장성 확보
- **Delegate 패턴**을 통한 느슨한 결합

---

## 📊 통계

**총 개발 일수**: 13일 (10.15 ~ 10.31)
**총 C++ 클래스 수**: 17개
**총 커밋 수**: 10+회
**네트워크 RPC 함수**: 8개
**UI 위젯**: 7개
**DataTable**: 3개 (DT_Fireman, DT_People, DT_Jinsang)

---

## 🙏 회고

### 배운 점
1. **언리얼 멀티플레이어 네트워킹**
   - Replication, RPC, OnRep의 차이와 적절한 사용법
   - ServerTravel을 통한 맵 전환
   - HasAuthority() 체크의 중요성

2. **게임 프레임워크 설계**
   - GameMode-GameState-PlayerState 구조의 장점
   - Phase 기반 게임 흐름 관리
   - Delegate를 통한 이벤트 기반 프로그래밍

3. **성능 최적화**
   - Tick 대신 Timer 사용하여 CPU 부하 감소
   - 네트워크 트래픽 최소화를 위한 RPC 설계

### 어려웠던 점
1. **순환 호출 버그** (10.17)
   - PlayerState ↔ GameMode 간 무한 반복 호출
   - 디버깅 및 함수 호출 흐름 재설계로 해결

2. **역할별 스폰 위치 이슈** (10.23)
   - ServerTravel 시 PlayerState 동기화 타이밍 문제
   - Key 저장 방식 개선 필요

3. **멀티플레이 동기화**
   - 클라이언트와 서버 간 상태 불일치
   - Replicated Property와 RPC의 적절한 조합으로 해결

### 개선 방향
- FireGameInstance Key 방식 개선 (NetPlayerIndex 기반)
- 역할별 스폰 위치 완전 해결
- NPC AI 시스템 확장 (StateTree 연동)

---

**프로젝트 종료일**: 2025.10.31
**담당자**: cubee021
**브랜치**: Cubee → main Merge 완료 🎉
