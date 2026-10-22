# MeasureTool — 측정 도구

이 폴더의 코드와 측정 결과는 AI(Claude, Anthropic)가 작성하고 측정한 것이다. 게임 소스(`dontstarveCopy/`)가 아니며 게임 빌드에 들어가지 않는다.

- 역할 나눔:
  - 사용자: 설계 규칙과 단계별 조건을 정한다.
  - 측정 도구: 게임 코드가 그 조건대로 동작하는지 칸 단위로 잰다.
- 근거로 쓰는 수치: 진화 기록 [docs/WorldGen.md](../docs/WorldGen.md)의 결과 수치는 모두 이 도구의 출력이다.

## WorldGen

게임의 `Core/WorldGenerator.cpp`를 그대로 함께 컴파일해 측정한다. 그래서 잰 것은 게임에 들어간 생성기 자체다.

- 생성기가 쓰는 정의 표(`Core/World.cpp`의 세계 타입 선언, `Core/Biome.cpp`, `Core/Tile.cpp`)와 `Core/Terrain.cpp`도 함께 컴파일한다. 측정은 Forest 세계 타입으로 한다.
- `World.cpp`와 `Tile.cpp`에는 엔진(CameraMgr, PathMgr, Core)을 부르는 그리기 코드가 같이 있다. 도구는 그리기를 부르지 않으므로 링크에서 그 참조를 비워 둔다(`/FORCE:UNRESOLVED`). 그래서 `bin/compile.log`에 그리기 쪽 LNK2019 줄과 LNK4088 경고가 남지만 실행 파일은 만들어진다. 그 함수가 불리면 도구가 멈춘다.

- 생성기에 시드를 넣어 부르고, 돌려받은 성공 여부와 생성기가 채운 `worldMap`만 잰다. 생성기 안의 중간값은 보지 않는다.
- 단계 조건의 기대값(몫, 원점 자리, 예상 원)은 진화 기록의 규칙에서 도구가 따로 계산한다.

```
MeasureTool\WorldGen\build.cmd L1_0
```

| 단계 이름 | 재는 것 |
|---|---|
| `L1_0` | 성공 여부, 시드 1~10, 땅 35% 이상, 몫, 섬, A 보장, 월경지, 내부 구멍, 예상 원 밖 칸, 무게중심 |

| 출력 | 위치 |
|---|---|
| 측정표 | `MeasureTool/WorldGen/results/<단계>.txt` |
| 지도 그림 | `docs/WorldGen/<단계>.png` (바이옴 색, 흰 원 = 규칙에서 계산한 예상 모양, 검은 점 = 규칙상 원점) |
| 빌드 산출물 | `MeasureTool/WorldGen/bin/` (저장소에서 무시됨) |
