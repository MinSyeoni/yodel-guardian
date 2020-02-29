#include "pch.h"
#include "ObjMgr.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#include "Terrain.h"
#include "Defines.h"
#include "Engine_Defines.h"


IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr(void)
{
}

CObjMgr::~CObjMgr(void)
{
	Release();
}

void CObjMgr::Load_Object(const _tchar* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte = 0;
	while (true)
	{


	}
	CloseHandle(hFile);
}

void CObjMgr::AddObject(Engine::CGameObject * pObject, OBJ_ID eId)
{
	m_ObjLst[eId].push_back(pObject);
}

void CObjMgr::Update_Object(const _float& fTimeDelta)
{	
	for (int i = 0; i < OBJ_END; ++i)
	{
		auto iter_begin = m_ObjLst[i].begin();

		for (; iter_begin != m_ObjLst[i].end();)
		{
			int iEvent = (*iter_begin)->Update_Object(fTimeDelta);

			if (1 == iEvent)
			{
				Engine::Safe_Delete(*iter_begin);
				iter_begin = m_ObjLst[i].erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}
}

void CObjMgr::Render_Object()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto &pObject : m_ObjLst[i])
			pObject->Render_Object();
	}
}

void CObjMgr::ReleaseGroup(OBJ_ID _id)
{
	auto iter_begin = m_ObjLst[_id].begin();
	for (; iter_begin != m_ObjLst[_id].end();)
	{
		Engine::Safe_Release(*iter_begin);
		iter_begin = m_ObjLst[_id].erase(iter_begin);
	}
	m_ObjLst[_id].clear();
}

Engine::CGameObject * CObjMgr::GetGameObject(OBJ_ID eId)
{
	if (m_ObjLst[eId].size()!=0)
		return m_ObjLst[eId].front();

	return nullptr;
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (int i = 0; i < OBJ_END; ++i)
		{
			auto iter_begin = m_ObjLst[i].begin();

			for (; iter_begin != m_ObjLst[i].end();)
			{
				unsigned long dwRefCnt = 0;
				dwRefCnt=(*iter_begin)->Release();

				if (0 == dwRefCnt)
					(*iter_begin)= NULL;

				iter_begin = m_ObjLst[i].erase(iter_begin);
			}
		}
	}
}
