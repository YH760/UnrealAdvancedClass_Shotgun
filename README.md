# MyProject2 — Unreal Advanced Class (Shotgun)

언리얼 엔진 C++ 학습 프로젝트입니다. 3인칭 캐릭터에 **샷건 발사 시스템**, **데미지/체력 시스템**, **HUD(체력 바)**, **사망/래그돌** 처리를 단계적으로 구현합니다.

---

## 주요 기능

- **샷건 무기 시스템** — 라인 트레이스 기반 발사, 펠릿 산탄, 머즐 이펙트/사운드, 카메라 반동
- **데미지 컴포넌트** — 체력 관리, 피격/사망 델리게이트 방송
- **체력 HUD** — `AHUD` 기반 HUDManager가 위젯을 생성·관리, 체력 바 + 텍스트 표시
- **사망 처리** — 체력 0 시 입력 차단 + 물리 기반 래그돌

---

## 아키텍처

```
AMyProject2Character (플레이어)
   ├─ DamageComponent ──(OnHealthDamaged)──► HUDManager ──► PlayerHUDWidget (체력 바)
   │                  └─(OnHealthDead)──────► Character::OnDead (래그돌)
   └─ AShotgunWeapon (장착 무기, WeaponSocket에 부착)

AHUDManager (AHUD)  : UI 생성·관리의 단일 진입점 (캐릭터/컨트롤러는 UI 코드 없음)
GameMode            : BP_GameMode → HUDClass = BP_HUDManager
```

### 설계 원칙
- **UI 생성은 HUDManager 한 곳**에 모음 — 캐릭터/컨트롤러를 UI 코드로 오염시키지 않음
- **데이터는 컴포넌트, 반응은 구독자** — `DamageComponent`가 체력을 관리하고, HUD·캐릭터가 델리게이트를 구독해 각자 반응
- 초기 체력은 HUDManager가 `DamageComponent::InitHealth()`를 호출해 **실행 순서에 의존하지 않게** 동기화

---

## 소스 구조

| 파일 | 역할 |
|------|------|
| `MyProject2Character` | 3인칭 캐릭터, 입력, 무기 장착, 카메라 반동, 사망/래그돌 |
| `MyProject2GameMode` | 기본 폰/HUD 클래스 지정 |
| `Public/WeaponBase` · `Private/WeaponBase` | 무기 베이스(발사·트레이스·반동 공통 로직) |
| `Public/ShotgunWeapon` · `Private/ShotgunWeapon` | 샷건(펠릿 산탄) 구현 |
| `Public/DamageComponent` · `Private/DamageComponent` | 체력 관리, 피격/사망 델리게이트 |
| `Widget/HUDManager` | 위젯 생성·관리, 데미지 델리게이트 구독 |
| `Widget/PlayerHUDWidget` | 체력 바 + 텍스트 표시 위젯 |
| `Widget/DamageNumberWidget` | 떠오르는 데미지 숫자 위젯 (작업 예정) |

---

## 빌드 & 실행

1. `MyProject2.uproject` 우클릭 → **Generate Visual Studio project files**
2. Visual Studio에서 빌드 (`Ctrl + Shift + B`)
3. 언리얼 에디터에서 프로젝트 열고 **Play**

> 핫 리로드가 실패하면 에디터를 닫고 Visual Studio에서 빌드한 뒤 다시 실행하세요.

### 에디터 설정
- **Project Settings → Maps & Modes → Default GameMode** = `BP_GameMode`
- `BP_GameMode.HUDClass` = `BP_HUDManager`
- `BP_HUDManager.PlayerHUDClass` = `WBP_PlayerHpHUD`
- `WBP_PlayerHpHUD`의 위젯 이름: `HealthBar`(ProgressBar), `HealthText`(TextBlock) — C++ `BindWidget` 변수명과 일치해야 함

---

## 핵심 흐름

**피격 → 체력 바 갱신**
```
ApplyDamage → DamageComponent::DamageTake → OnHealthDamaged.Broadcast
            → HUDManager::HandleHealthDamaged → PlayerHUDWidget::OnHealthChanged
```

**체력 0 → 사망(래그돌)**
```
DamageComponent::DamageTake (HP==0) → OnHealthDead.Broadcast
            → Character::OnDead → 입력 차단 / 무브먼트 정지 / 캡슐 충돌 off / 메시 물리 on
```
