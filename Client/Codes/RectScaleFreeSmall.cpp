#include "stdafx.h"
#include "RectScaleFreeSmall.h"
#include "Camera.h"

#include "SoundMgr.h"

CRectScaleFreeSmall::CRectScaleFreeSmall(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pVIBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{
}

CRectScaleFreeSmall::CRectScaleFreeSmall(const CRectScaleFreeSmall & other)
	: CGameObject(other), m_pVIBufferCom(other.m_pVIBufferCom), m_pTransformCom(other.m_pTransformCom), m_pTextureCom(other.m_pTextureCom)
{
}

HRESULT CRectScaleFreeSmall::Ready_GameObject_Prototype()
{
	CGameObject::Ready_GameObject_Prototype();
	return S_OK;
}

HRESULT CRectScaleFreeSmall::Ready_GameObject(void * pArg)
{
	CGameObject::Ready_GameObject(pArg);

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_VIBuffer_RectTexture", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_VIBuffer");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::NoneStatic, L"Component_RectScaleFreeSmall", L"Com_Texture", (CComponent**)&m_pTextureCom)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Texture");
		return E_FAIL;
	}

	if (FAILED(Add_Component(ePrototypeType::Static, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom, pArg)))
	{
		PRINT_LOG(L"Error", L"Failed to Add Com_Transform");
		return E_FAIL;
	}


	
	return S_OK;
}

_uint CRectScaleFreeSmall::Update_GameObject(_float fDeltaTime)
{
	CGameObject::Update_GameObject(fDeltaTime);

	if (FAILED(Movement(fDeltaTime)))
	{
		PRINT_LOG(L"Error", L"Failed To Movement Mouse");
		return UPDATE_ERROR;
	}

	if (FAILED(m_pTransformCom->Update_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed To Update_Transform");
		return UPDATE_ERROR;
	}
	return S_OK;
}

_uint CRectScaleFreeSmall::Late_Update_GameObject(_float fDeltaTime)
{



	CGameObject::Late_Update_GameObject(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (!pManagement)
	{
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return UPDATE_ERROR;
	}

//	m_iIndex = pManagement->Get_Index();

	if (FAILED(pManagement->Add_GameObjectInRenderer(eRenderType::UI, this)))
	{
		PRINT_LOG(L"Error", L"Failed To Add Mouse In Renderer");
		return UPDATE_ERROR;
	}

	return S_OK;
}

HRESULT CRectScaleFreeSmall::Render_GameObject()
{
	CGameObject::Render_GameObject();


	if (FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransformCom->Get_Desc().matWorld))) {
		PRINT_LOG(L"Error", L"Failed To SetTransform Mouse");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Set_Texture())) {
		PRINT_LOG(L"Error", L"Failed To Set_Texture Mouse");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render_VIBuffer())) {
		PRINT_LOG(L"Error", L"Failed To Render_VIBuffer Mouse");
		return E_FAIL;
	}


	return S_OK;
}

CRectScaleFreeSmall * CRectScaleFreeSmall::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRectScaleFreeSmall*	pInstance = new CRectScaleFreeSmall(pDevice);
	if (FAILED(pInstance->Ready_GameObject_Prototype())) {
		PRINT_LOG(L"Error", L"Failed To Create Mouse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRectScaleFreeSmall::Clone(void * pArg)
{
	CRectScaleFreeSmall*	pClone = new CRectScaleFreeSmall(*this);
	if (FAILED(pClone->Ready_GameObject(pArg))) {
		PRINT_LOG(L"Error", L"Failed To Clone Mouse");
		Safe_Release(pClone);
	}

	return pClone;
}

void CRectScaleFreeSmall::Free()
{
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}

HRESULT CRectScaleFreeSmall::Movement(_float fDeltaTime)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (!pManagement) {
		PRINT_LOG(L"Error", L"pManagement is nullptr");
		return E_FAIL;
	}

	TRANSFORM_DESC temp = m_pTransformCom->Get_Desc();

	/* 대기 */
	if (m_bCnt) {

		/* 카운트 */
		m_fCnt += (fDeltaTime * 2);
		if (m_fCnt > 8.f){
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
			CSoundMgr::Get_Instance()->PlaySound(L"Menu_Appear.wav", CSoundMgr::UI);
			m_bCnt = false;
			temp.vPos.y = 0.3f;
			m_bScaling = true; }
	}

	if (m_bScaling) {
		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
	//	ShowCursor(FALSE);

		_vec3 vMousePos = { (2 * (float)pt.x / WINCX) - 1, (-2 * (float)pt.y / WINCY) + 1, 0.f };

		/* 마우스 충돌*/
		if (vMousePos.x > -0.15f && vMousePos.x < 0.15f){

			if (vMousePos.y >= 0.16f && vMousePos.y <= 0.36f){
				m_bMoveUp = true;
				m_bMove = true;
			}

			if (vMousePos.y >= -0.4f && vMousePos.y <= -0.25f)	{
				m_bMoveDown = true;
				m_bMove = true;

			}
		}
	}


	if (m_bMove) {
		if (m_bMoveUp) {
			if (temp.vPos.y < 0.3f) {
				temp.vPos.y += (fDeltaTime);
				CSoundMgr::Get_Instance()->PlaySound(L"Menu_Change.mp3", CSoundMgr::UI);
			}
			else {
				m_bMoveUp = false;
				m_bMove = false;


			}
		}
		if (m_bMoveDown) {
			if (temp.vPos.y > -0.3f)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Menu_Change.mp3", CSoundMgr::UI);
				temp.vPos.y -= (fDeltaTime);
			}
			else {
				m_bMoveDown = false;
				m_bMove = false;

			}
		}

	}


	if (GetAsyncKeyState(VK_RETURN)){
		m_bClose = true;
	}
	if (m_bClose){
		temp.vScale *= (1 - fDeltaTime * 10.f);
	}

	m_pTransformCom->Set_Desc(temp);
	return S_OK;
}


