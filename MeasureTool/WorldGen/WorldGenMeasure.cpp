// AI(Claude, Anthropic)가 작성한 측정 도구다. 게임 소스가 아니며 게임 빌드에 들어가지 않는다.
// 게임의 Core/WorldGenerator.cpp를 그대로 컴파일해 시드를 넣어 부르고, 성공 여부와 생성기가 채운 worldMap만 칸 단위로 잰다.
// 단계 조건의 기대값(몫, 원점 자리)은 생성기 안을 보지 않고 docs/WorldGen.md의 규칙에서 이 도구가 따로 계산한다.
// 사용: WorldGenMeasure.exe <단계 이름(L1_0)> <결과 txt 경로> <지도 png 경로>
#include "pch.h"
#include "Core/WorldGenerator.h"
#include <chrono>
#include <cstdarg>
#include <fcntl.h>
#include <io.h>
#include <cmath>
#include <cstdio>
#include <set>

namespace
{
    const double pi = 3.14159265358979323846;
    const int steps[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
    const int water = (int)BiomeType::Water;
    const int spawn = (int)BiomeType::Grass;
    const int typeCount = (int)BiomeType::END;

    // 게임의 World 바이옴 색과 같다
    const Color biomeColors[] =
    {
        Color(255, 64, 120, 190), Color(255, 250, 199, 117), Color(255, 59, 109, 17), Color(255, 200, 180, 70), Color(255, 160, 95, 40),
        Color(255, 90, 80, 110), Color(255, 235, 160, 90), Color(255, 151, 196, 89), Color(255, 140, 140, 140),
    };
    const wchar_t* biomeNames[] = { L"Water", L"Grass", L"Forest", L"Savanna", L"Deciduous", L"Marsh", L"Desert", L"MandrakeForest", L"Rocky" };

    // 게임 생성기에 시드를 넣어 부르고, 칸마다 바이옴 번호를 (y * 크기 + x) 순서로 돌려준다
    vector<int> Generate(unsigned int seed, int worldSize, bool& outIsGenerated, double& outMilliseconds)
    {
        vector<vector<Terrain>> worldMap(worldSize, vector<Terrain>(worldSize, Terrain(BiomeType::Water, TileType::OceanCoastal)));
        auto startTime = chrono::steady_clock::now();
        WorldGenerator generator;
        outIsGenerated = generator.generateWorld(worldMap, WorldType::Forest, seed);
        outMilliseconds = chrono::duration<double, milli>(chrono::steady_clock::now() - startTime).count();

        vector<int> types(worldSize * worldSize);
        for (int y = 0; y < worldSize; ++y)
        {
            for (int x = 0; x < worldSize; ++x)
            {
                types[y * worldSize + x] = (int)worldMap[x][y].getBiomeType();
            }
        }
        return types;
    }

    // type이 water면 물이 아닌 칸 전체(섬)를, 아니면 그 바이옴 칸만 같은 덩어리로 본다
    bool IsSamePart(int cellType, int type)
    {
        return type == water ? cellType != water : cellType == type;
    }

    // 상하좌우로 이어진 덩어리 수
    int CountParts(const vector<int>& types, int worldSize, int type)
    {
        int parts = 0;
        vector<char> isVisited(types.size(), 0);
        for (int cell = 0; cell < (int)types.size(); ++cell)
        {
            if (IsSamePart(types[cell], type) == false || isVisited[cell])
            {
                continue;
            }
            ++parts;
            vector<int> stack = { cell };
            isVisited[cell] = 1;
            while (stack.empty() == false)
            {
                int current = stack.back();
                stack.pop_back();
                for (const auto& step : steps)
                {
                    int nextX = current % worldSize + step[0];
                    int nextY = current / worldSize + step[1];
                    if (nextX < 0 || nextX >= worldSize || nextY < 0 || nextY >= worldSize)
                    {
                        continue;
                    }
                    int next = nextY * worldSize + nextX;
                    if (IsSamePart(types[next], type) && isVisited[next] == 0)
                    {
                        isVisited[next] = 1;
                        stack.push_back(next);
                    }
                }
            }
        }
        return parts;
    }

    // from에서 땅만 밟아 닿는 칸 수
    int CountReached(const vector<int>& types, int worldSize, POINT from)
    {
        int start = from.y * worldSize + from.x;
        if (types[start] == water)
        {
            return 0;
        }
        int reached = 0;
        vector<char> isReached(types.size(), 0);
        vector<int> open = { start };
        isReached[start] = 1;
        while (open.empty() == false)
        {
            int cell = open.back();
            open.pop_back();
            ++reached;
            for (const auto& step : steps)
            {
                int nextX = cell % worldSize + step[0];
                int nextY = cell / worldSize + step[1];
                if (nextX < 0 || nextX >= worldSize || nextY < 0 || nextY >= worldSize)
                {
                    continue;
                }
                int next = nextY * worldSize + nextX;
                if (types[next] != water && isReached[next] == 0)
                {
                    isReached[next] = 1;
                    open.push_back(next);
                }
            }
        }
        return reached;
    }

    // A의 보장: 외곽 원점에서 중앙 원점까지 중앙 쪽으로만 움직이는 상하좌우 최단 경로 중, 두 바이옴의 땅만 밟는 것이 있는가
    bool HasLandShortestPath(const vector<int>& types, int worldSize, int type, POINT from, POINT to)
    {
        int width = abs(to.x - from.x) + 1;
        int height = abs(to.y - from.y) + 1;
        int directionX = to.x >= from.x ? 1 : -1;
        int directionY = to.y >= from.y ? 1 : -1;
        vector<char> isReached(width * height, 0);
        for (int row = 0; row < height; ++row)
        {
            for (int column = 0; column < width; ++column)
            {
                int x = from.x + column * directionX;
                int y = from.y + row * directionY;
                int cellType = types[y * worldSize + x];
                if (cellType != type && cellType != spawn)
                {
                    continue;
                }
                bool isStart = row == 0 && column == 0;
                bool fromLeft = column > 0 && isReached[row * width + column - 1];
                bool fromUp = row > 0 && isReached[(row - 1) * width + column];
                isReached[row * width + column] = (isStart || fromLeft || fromUp) ? 1 : 0;
            }
        }
        return isReached[width * height - 1] != 0;
    }

    // 측정표 한 줄을 콘솔과 결과 파일(BOM 없는 UTF-8)에 같이 쓴다
    void PrintLine(FILE* report, const wchar_t* format, ...)
    {
        wchar_t line[1024];
        va_list arguments;
        va_start(arguments, format);
        _vsnwprintf_s(line, _TRUNCATE, format, arguments);
        va_end(arguments);
        fputws(line, stdout);
        if (report != nullptr)
        {
            char bytes[4096];
            int length = WideCharToMultiByte(CP_UTF8, 0, line, -1, bytes, sizeof(bytes), nullptr, nullptr);
            fwrite(bytes, 1, length > 0 ? length - 1 : 0, report);
        }
    }

    bool FindPngEncoder(CLSID& outEncoderId)
    {
        UINT count = 0;
        UINT size = 0;
        GetImageEncodersSize(&count, &size);
        vector<BYTE> buffer(size);
        ImageCodecInfo* encoders = (ImageCodecInfo*)buffer.data();
        GetImageEncoders(count, size, encoders);
        for (UINT i = 0; i < count; ++i)
        {
            if (wcscmp(encoders[i].MimeType, L"image/png") == 0)
            {
                outEncoderId = encoders[i].Clsid;
                return true;
            }
        }
        return false;
    }

    // 생성된 지도 + 규칙에서 계산한 예상 모양(반지름 r) 테두리와 규칙상 원점
    void SaveMap(const wchar_t* path, const vector<int>& types, const vector<POINT>& origins, const vector<double>& radii, int worldSize)
    {
        Bitmap bitmap(worldSize, worldSize, PixelFormat32bppARGB);
        for (int y = 0; y < worldSize; ++y)
        {
            for (int x = 0; x < worldSize; ++x)
            {
                bitmap.SetPixel(x, y, biomeColors[types[y * worldSize + x]]);
            }
        }
        Graphics graphics(&bitmap);
        graphics.SetSmoothingMode(SmoothingModeAntiAlias);
        Pen outline(Color(160, 255, 255, 255), 1.0f);
        SolidBrush originBrush(Color(255, 0, 0, 0));
        for (int index = 0; index < (int)origins.size(); ++index)
        {
            REAL radius = (REAL)radii[index];
            graphics.DrawEllipse(&outline, (REAL)origins[index].x - radius, (REAL)origins[index].y - radius, radius * 2, radius * 2);
            graphics.FillEllipse(&originBrush, (REAL)origins[index].x - 2.5f, (REAL)origins[index].y - 2.5f, 5.0f, 5.0f);
        }
        CLSID encoderId;
        if (FindPngEncoder(encoderId))
        {
            bitmap.Save(path, &encoderId, nullptr);
        }
    }
}

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 4 || wcscmp(argv[1], L"L1_0") != 0)
    {
        fwprintf(stderr, L"사용: WorldGenMeasure.exe <단계 이름(L1_0)> <결과 txt 경로> <지도 png 경로>\n");
        return 2;
    }
    const wchar_t* levelName = argv[1];
    int worldSize = WSREGULAR;
    int cellCount = worldSize * worldSize;
    POINT center = { worldSize / 2, worldSize / 2 };

    GdiplusStartupInput startupInput;
    ULONG_PTR gdiplusToken = 0;
    GdiplusStartup(&gdiplusToken, &startupInput, nullptr);

    bool isGenerated = false;
    bool isGeneratedAgain = false;
    double elapsedMilliseconds = 0.0;
    double unusedMilliseconds = 0.0;
    vector<int> types = Generate(1, worldSize, isGenerated, elapsedMilliseconds);
    bool isSameAgain = Generate(1, worldSize, isGeneratedAgain, unusedMilliseconds) == types;

    // 시드 1~10: 성공을 돌려주는가, 시드 1과 같은 지도인가
    int generatedSeedCount = 0;
    int sameSeedCount = 0;
    for (unsigned int seed = 1; seed <= 10; ++seed)
    {
        bool isSeedGenerated = false;
        vector<int> seedTypes = Generate(seed, worldSize, isSeedGenerated, unusedMilliseconds);
        generatedSeedCount += isSeedGenerated ? 1 : 0;
        sameSeedCount += seedTypes == types ? 1 : 0;
    }

    // 시드 1 지도의 바이옴별 칸, 무게중심, 맞닿은 변
    int landCount = 0;
    vector<int> placed(typeCount, 0);
    vector<double> sumX(typeCount, 0.0);
    vector<double> sumY(typeCount, 0.0);
    vector<int> spawnEdges(typeCount, 0);
    vector<set<int>> neighbors(typeCount);
    for (int cell = 0; cell < cellCount; ++cell)
    {
        int type = types[cell];
        if (type == water)
        {
            continue;
        }
        ++landCount;
        ++placed[type];
        int x = cell % worldSize;
        int y = cell / worldSize;
        sumX[type] += x;
        sumY[type] += y;
        for (const auto& step : steps)
        {
            int nextX = x + step[0];
            int nextY = y + step[1];
            if (nextX < 0 || nextX >= worldSize || nextY < 0 || nextY >= worldSize)
            {
                neighbors[type].insert(-1);
                continue;
            }
            int next = types[nextY * worldSize + nextX];
            if (next == type)
            {
                continue;
            }
            neighbors[type].insert(next);
            if (type != spawn && next == spawn)
            {
                ++spawnEdges[type];
            }
        }
    }
    vector<int> parts(typeCount, 0);
    for (int type = 1; type < typeCount; ++type)
    {
        parts[type] = placed[type] > 0 ? CountParts(types, worldSize, type) : 0;
    }
    int islandCount = CountParts(types, worldSize, water);
    int reachedCount = CountReached(types, worldSize, center);

    // L1.0 규칙에서 따로 계산한 기대값: 바이옴 8개(시작 바이옴 먼저), 몫 = 땅 35% ÷ 8 올림, 원으로 가정한 반지름, 등간격으로 맞닿는 원점
    int landTarget = cellCount * 35 / 100;
    int biomeCount = 8;
    int quota = (landTarget + biomeCount - 1) / biomeCount;
    double radius = sqrt(quota / pi);
    vector<int> ruleTypes;
    vector<POINT> ruleOrigins;
    vector<double> ruleRadii;
    vector<double> ruleAngles;
    for (int index = 0; index < biomeCount; ++index)
    {
        ruleTypes.push_back(spawn + index);
        ruleRadii.push_back(radius);
        POINT origin = center;
        double angle = 0.0;
        if (index > 0)
        {
            angle = (index - 1) * 2.0 * pi / (biomeCount - 1);
            origin.x = center.x + lround(cos(angle) * (radius + radius));
            origin.y = center.y + lround(sin(angle) * (radius + radius));
        }
        ruleOrigins.push_back(origin);
        ruleAngles.push_back(angle);
    }

    int quotaMissCount = 0;
    int enclaveCount = 0;
    int holeCount = 0;
    int pathBrokenCount = 0;
    for (int index = 0; index < biomeCount; ++index)
    {
        int type = ruleTypes[index];
        POINT origin = ruleOrigins[index];
        if (placed[type] != quota)
        {
            ++quotaMissCount;
        }
        if (parts[type] != 1 || types[origin.y * worldSize + origin.x] != type)
        {
            ++enclaveCount;
        }
        if (neighbors[type].size() == 1 && *neighbors[type].begin() != water && *neighbors[type].begin() != -1)
        {
            ++holeCount;
        }
        if (index > 0 && HasLandShortestPath(types, worldSize, type, origin, center) == false)
        {
            ++pathBrokenCount;
        }
    }

    _setmode(_fileno(stdout), _O_U8TEXT);
    FILE* report = _wfopen(argv[2], L"wb");
    PrintLine(report, L"# %ls 측정 결과 (측정 도구 MeasureTool/WorldGen)\n\n", levelName);
    PrintLine(report, L"게임 생성기에 시드를 넣어 부르고, 성공 여부와 생성기가 채운 worldMap(%d x %d)만 쟀다. 기대값은 docs/WorldGen.md의 규칙에서 도구가 따로 계산했다.\n\n", worldSize, worldSize);
    PrintLine(report, L"| 항목 | 결과 |\n|---|---|\n");
    PrintLine(report, L"| 생성 (시드 1) | %ls |\n", isGenerated ? L"성공" : L"실패");
    PrintLine(report, L"| 생성 시간 (시드 1) | %.1f ms |\n", elapsedMilliseconds);
    PrintLine(report, L"| 같은 시드로 두 번 생성 | %ls |\n", isSameAgain ? L"칸 단위로 같음" : L"다름");
    PrintLine(report, L"| 시드 1~10에서 성공을 돌려준 수 | %d / 10 |\n", generatedSeedCount);
    PrintLine(report, L"| 시드 1~10의 지도가 시드 1과 같은가 | %d / 10 |\n", sameSeedCount);
    PrintLine(report, L"| 땅 | %d칸, %.2f%% |\n", landCount, landCount * 100.0 / cellCount);
    PrintLine(report, L"| 땅 목표 (35%% 이상) | %d칸 이상 → %ls |\n", landTarget, landCount >= landTarget ? L"충족" : L"미달");
    PrintLine(report, L"| 몫 (바이옴마다 %d칸 = %d ÷ %d 올림) | 바이옴 %d개 중 어긋남 %d개 |\n", quota, landTarget, biomeCount, biomeCount, quotaMissCount);
    PrintLine(report, L"| 섬 = 땅 덩어리 | %d |\n", islandCount);
    PrintLine(report, L"| 중앙 칸에서 땅만 밟아 닿는 칸 | %d / %d |\n", reachedCount, landCount);
    PrintLine(report, L"| A 보장 (외곽 원점 ~ 중앙 원점 최단 경로가 두 바이옴의 땅) | 외곽 %d개 중 위반 %d개 |\n", biomeCount - 1, pathBrokenCount);
    PrintLine(report, L"| 월경지 (조각 분리, 또는 규칙상 원점 칸이 자기 땅이 아님) | %d |\n", enclaveCount);
    PrintLine(report, L"| 내부 구멍 (다른 바이옴 하나에만 둘러싸임) | %d |\n\n", holeCount);

    PrintLine(report, L"| 놓은 순서 | 바이옴 | 칸 | 조각 | 규칙상 원점 | 예상 원(r %.1f) 밖 칸 | 시작 바이옴과 맞닿은 변 | 무게중심 각도 (규칙) | 무게중심 거리 (규칙 %.1f) |\n", radius, radius + radius);
    PrintLine(report, L"|---|---|---|---|---|---|---|---|---|\n");
    for (int index = 0; index < biomeCount; ++index)
    {
        int type = ruleTypes[index];
        POINT origin = ruleOrigins[index];
        int outside = 0;
        for (int cell = 0; cell < cellCount; ++cell)
        {
            if (types[cell] == type && hypot((double)(cell % worldSize - origin.x), (double)(cell / worldSize - origin.y)) > ruleRadii[index])
            {
                ++outside;
            }
        }
        double centroidX = placed[type] > 0 ? sumX[type] / placed[type] : center.x;
        double centroidY = placed[type] > 0 ? sumY[type] / placed[type] : center.y;
        double centroidDistance = hypot(centroidX - center.x, centroidY - center.y);
        double centroidAngle = atan2(centroidY - center.y, centroidX - center.x) * 180.0 / pi;
        if (centroidAngle < -0.5)
        {
            centroidAngle += 360.0;
        }
        wstring edgeText = index == 0 ? L"-" : to_wstring(spawnEdges[type]);
        wchar_t angleText[64] = L"-";
        if (index > 0)
        {
            swprintf_s(angleText, L"%.1f (%.1f)", centroidAngle, ruleAngles[index] * 180.0 / pi);
        }
        PrintLine(report, L"| %d | %ls | %d | %d | (%d, %d) | %d | %ls | %ls | %.1f |\n", index, biomeNames[type], placed[type], parts[type],
            origin.x, origin.y, outside, edgeText.c_str(), angleText, centroidDistance);
    }
    if (report != nullptr)
    {
        fclose(report);
    }

    SaveMap(argv[3], types, ruleOrigins, ruleRadii, worldSize);
    GdiplusShutdown(gdiplusToken);
    return isGenerated ? 0 : 1;
}
