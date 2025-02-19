#pragma once

class Res;

class Res
{
private:
    wstring m_resID;
    wstring m_resPath;

public:
    Res();
    virtual ~Res();
    
    const wstring& GetKey() { return m_resID; }
    const wstring& GetPath() { return m_resPath; }
    
    void SetKey(const wstring& resID) { m_resID = resID; }
    void SetPath(const wstring& resPath) { m_resPath = resPath; }
};
