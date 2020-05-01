#ifndef Engine_Struct_h__
#define Engine_Struct_h__

using namespace std;
using namespace Engine;
using namespace DirectX;

/*____________________________________________________________________
[ VECTOR2 ]
______________________________________________________________________*/
typedef struct tagVector2 : public XMFLOAT2
{
    explicit tagVector2() : XMFLOAT2(0.f, 0.f) {}
    explicit tagVector2(const float& x, const float& y) : XMFLOAT2(x, y) {}
    explicit tagVector2(const float& input) : XMFLOAT2(input, input) {}
    explicit tagVector2(const XMFLOAT2& input) : XMFLOAT2(input) {}

    /*____________________________________________________________________
    연산자 오버로딩
    ______________________________________________________________________*/
    // 덧셈
    tagVector2 operator+(const tagVector2& input) { return tagVector2(this->x + input.x, this->y + input.y); }

    // 뺄셈
    tagVector2 operator-(const tagVector2& input) { return tagVector2(this->x - input.x, this->y - input.y); }

    // 곱셈
    tagVector2 operator*(const tagVector2& input) { return tagVector2(this->x * input.x, this->y * input.y); }
    tagVector2 operator*(const int& input) { return tagVector2((float)input * this->x, (float)input * this->y); }
    tagVector2 operator*(const float& input) { return tagVector2(input * this->x, input * this->y); }
    tagVector2 operator*(const double& input) { return tagVector2((float)input * this->x, (float)input * this->y); }
    friend tagVector2 operator*(const int& input, tagVector2& v1) { return tagVector2((float)input * v1.x, (float)input * v1.y); }
    friend tagVector2 operator*(const float& input, tagVector2& v1) { return tagVector2(input * v1.x, input * v1.y); }
    friend tagVector2 operator*(const double& input, tagVector2& v1) { return tagVector2((float)input * v1.x, (float)input * v1.y); }

    // 축약 연산자
    tagVector2 operator+=(const tagVector2& input) { return (*this) = (*this) + input; }
    tagVector2 operator-=(const tagVector2& input) { return (*this) = (*this) - input; }
    tagVector2 operator*=(const tagVector2& input) { return (*this) = (*this) * input; }
    tagVector2 operator*=(const int& input) { return (*this) = (*this) * input; }
    tagVector2 operator*=(const float& input) { return (*this) = (*this) * input; }
    tagVector2 operator*=(const double& input) { return (*this) = (*this) * input; }

    // 비교 연산자
    _bool operator==(const tagVector2& input)
    {
        XMVECTOR dst = XMVectorSet(input.x, input.y, 0.f, 0.f);
        XMVECTOR src = XMVectorSet(this->x, this->y, 0.f, 0.f);
        return XMVector2Equal(dst, src);
    }

    _bool operator!=(const tagVector2& input)
    {
        XMVECTOR dst = XMVectorSet(input.x, input.y, 0.f, 0.f);
        XMVECTOR src = XMVectorSet(this->x, this->y, 0.f, 0.f);
        return !XMVector2Equal(dst, src);
    }

    /*____________________________________________________________________
    Function.
    ______________________________________________________________________*/
    XMVECTOR Get_XMVECTOR()
    {
        return XMVectorSet(this->x, this->y, 0.f, 0.f);
    }

    void Normalize()
    {
        XMVECTOR temp = XMVectorSet(this->x, this->y, 0.f, 0.f);
        temp = XMVector2Normalize(temp);

        this->x = XMVectorGetX(temp);
        this->y = XMVectorGetY(temp);
    }

    float Get_Length()
    {
        XMVECTOR temp = XMVectorSet(this->x, this->y, 0.f, 0.f);
        temp = XMVector2Length(temp);

        return XMVectorGetX(temp);
    }

    float Get_Distance(const tagVector2& vDst)
    {
        XMVECTOR dst = XMVectorSet(vDst.x, vDst.y, 0.f, 0.f);
        XMVECTOR src = XMVectorSet(this->x, this->y, 0.f, 0.f);

        XMVECTOR result = dst - src;
        result = XMVector2Length(result);

        return XMVectorGetX(result);
    }

    float Get_Angle(const tagVector2& vDst)
    {
        XMVECTOR v1 = XMVectorSet(vDst.x, vDst.y, 0.f, 0.f);
        XMVECTOR v2 = XMVectorSet(this->x, this->y, 0.f, 0.f);

        XMVECTOR angle = XMVector2AngleBetweenVectors(v1, v2);
        float   angleRadians = XMVectorGetX(angle);

        return XMConvertToDegrees(angleRadians);
    }

    float Dot(const tagVector2& vDst)
    {
        XMVECTOR dst = XMVectorSet(vDst.x, vDst.y, 0.f, 0.f);
        XMVECTOR src = XMVectorSet(this->x, this->y, 0.f, 0.f);
        XMVECTOR result = XMVector2Dot(dst, src);
        return XMVectorGetX(result);
    }

    tagVector2 Cross_InputDst(const tagVector2& vDst)
    {
        XMVECTOR dst = XMVectorSet(vDst.x, vDst.y, 0.f, 0.f);
        XMVECTOR src = XMVectorSet(this->x, this->y, 0.f, 0.f);
        XMVECTOR result = XMVector2Cross(dst, src);

        return tagVector2(XMVectorGetX(result), XMVectorGetY(result));
    }

    tagVector2 Cross_InputSrc(const tagVector2& vSrc)
    {
        XMVECTOR dst = XMVectorSet(this->x, this->y, 0.f, 0.f);
        XMVECTOR src = XMVectorSet(vSrc.x, vSrc.y, 0.f, 0.f);
        XMVECTOR result = XMVector2Cross(dst, src);

        return tagVector2(XMVectorGetX(result), XMVectorGetY(result));
    }

    void Print() { cout << "x : " << this->x << "\t y : " << this->y << endl; }

}_vec2;

/*____________________________________________________________________
[ VECTOR3 ]
______________________________________________________________________*/
typedef struct tagVector3 : public XMFLOAT3
{
    explicit tagVector3() : XMFLOAT3(0.f, 0.f, 0.f) {}
    explicit tagVector3(const float& x, const float& y, const float& z) : XMFLOAT3(x, y, z) {}
    explicit tagVector3(const float& input) : XMFLOAT3(input, input, input) {}
    explicit tagVector3(const XMFLOAT3& input) : XMFLOAT3(input) {}

    /*____________________________________________________________________
    연산자 오버로딩
    ______________________________________________________________________*/
    // 덧셈
    tagVector3 operator+(const tagVector3& input) { return tagVector3(this->x + input.x, this->y + input.y, this->z + input.z); }

    // 뺄셈
    tagVector3 operator-(const tagVector3& input) { return tagVector3(this->x - input.x, this->y - input.y, this->z - input.z); }

    // 곱셈
    tagVector3 operator*(const tagVector3& input) { return tagVector3(this->x * input.x, this->y * input.y, this->z * input.z); }
    tagVector3 operator*(const int& input) { return tagVector3((float)input * this->x, (float)input * this->y, (float)input * this->z); }
    tagVector3 operator*(const float& input) { return tagVector3(input * this->x, input * this->y, input * this->z); }
    tagVector3 operator*(const double& input) { return tagVector3((float)input * this->x, (float)input * this->y, (float)input * this->z); }
    friend tagVector3 operator*(const int& input, tagVector3& v1) { return tagVector3((float)input * v1.x, (float)input * v1.y, (float)input * v1.z); }
    friend tagVector3 operator*(const float& input, tagVector3& v1) { return tagVector3(input * v1.x, input * v1.y, input * v1.z); }
    friend tagVector3 operator*(const double& input, tagVector3& v1) { return tagVector3((float)input * v1.x, (float)input * v1.y, (float)input * v1.z); }

    // 축약 연산자
    tagVector3 operator+=(const tagVector3& input) { return (*this) = (*this) + input; }
    tagVector3 operator-=(const tagVector3& input) { return (*this) = (*this) - input; }
    tagVector3 operator*=(const tagVector3& input) { return (*this) = (*this) * input; }
    tagVector3 operator*=(const int& input) { return (*this) = (*this) * input; }
    tagVector3 operator*=(const float& input) { return (*this) = (*this) * input; }
    tagVector3 operator*=(const double& input) { return (*this) = (*this) * input; }

    // 비교 연산자
    _bool operator==(const tagVector3& input)
    {
        XMVECTOR dst = XMVectorSet(input.x, input.y, input.z, 0.f);
        XMVECTOR src = XMVectorSet(this->x, this->y, this->z, 0.f);
        return XMVector3Equal(dst, src);
    }

    _bool operator!=(const tagVector3& input)
    {
        XMVECTOR dst = XMVectorSet(input.x, input.y, input.z, 0.f);
        XMVECTOR src = XMVectorSet(this->x, this->y, this->z, 0.f);
        return !XMVector3Equal(dst, src);
    }

    /*____________________________________________________________________
    Function.
    ______________________________________________________________________*/
    XMVECTOR Get_XMVECTOR()
    {
        return XMVectorSet(this->x, this->y, this->z, 0.f);
    }

    void Normalize()
    {
        XMVECTOR temp = XMVectorSet(this->x, this->y, this->z, 0.f);
        temp = XMVector3Normalize(temp);

        this->x = XMVectorGetX(temp);
        this->y = XMVectorGetY(temp);
        this->z = XMVectorGetZ(temp);
    }

    float Get_Length()
    {
        XMVECTOR temp = XMVectorSet(this->x, this->y, this->z, 0.f);
        temp = XMVector3Length(temp);

        return XMVectorGetX(temp);
    }

    float Get_Distance(const tagVector3& vDst)
    {
        XMVECTOR dst = XMVectorSet(vDst.x, vDst.y, vDst.z, 0.f);
        XMVECTOR src = XMVectorSet(this->x, this->y, this->z, 0.f);

        XMVECTOR result = dst - src;
        result = XMVector3Length(result);

        return XMVectorGetX(result);
    }

    float Get_Angle(const tagVector3& vDst)
    {
        XMVECTOR v1 = XMVectorSet(vDst.x, vDst.y, vDst.z, 0.f);
        XMVECTOR v2 = XMVectorSet(this->x, this->y, this->z, 0.f);

        XMVECTOR angle = XMVector3AngleBetweenVectors(v1, v2);
        float   angleRadians = XMVectorGetX(angle);

        return XMConvertToDegrees(angleRadians);
    }

    float Dot(const tagVector3& vDst)
    {
        XMVECTOR dst = XMVectorSet(vDst.x, vDst.y, vDst.z, 0.f);
        XMVECTOR src = XMVectorSet(this->x, this->y, this->z, 0.f);
        XMVECTOR result = XMVector3Dot(dst, src);
        return XMVectorGetX(result);
    }

    tagVector3 Cross_InputDst(const tagVector3& vDst)
    {
        XMVECTOR dst = XMVectorSet(vDst.x, vDst.y, vDst.z, 0.f);
        XMVECTOR src = XMVectorSet(this->x, this->y, this->z, 0.f);
        XMVECTOR result = XMVector3Cross(dst, src);

        return tagVector3(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result));
    }

    tagVector3 Cross_InputSrc(const tagVector3& vSrc)
    {
        XMVECTOR dst = XMVectorSet(this->x, this->y, this->z, 0.f);
        XMVECTOR src = XMVectorSet(vSrc.x, vSrc.y, vSrc.z, 0.f);
        XMVECTOR result = XMVector3Cross(dst, src);

        return tagVector3(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result));
    }

    void TransformCoord(const tagVector3& input, const _matrix& matrix)
    {
        XMVECTOR vIn = XMVectorSet(input.x, input.y, input.z, 1.f);
        XMVECTOR vOut = XMVector3TransformCoord(vIn, matrix);

        this->x = XMVectorGetX(vOut);
        this->y = XMVectorGetY(vOut);
        this->z = XMVectorGetZ(vOut);
    }

    void TransformNormal(const tagVector3& input, const _matrix& matrix)
    {
        XMVECTOR vIn = XMVectorSet(input.x, input.y, input.z, 0.f);
        XMVECTOR vOut = XMVector3TransformNormal(vIn, matrix);

        this->x = XMVectorGetX(vOut);
        this->y = XMVectorGetY(vOut);
        this->z = XMVectorGetZ(vOut);
    }

    tagVector3 Convert_2DWindowToDescartes(const _int& WINCX, const _int& WINCY)
    {
        /*____________________________________________________________________
        2D UI 사용 시, Window좌표를 입력해서 사용할 수 있도록
        윈도우 좌표를 데카르트 좌표로 변환해주는 좌표.
        ______________________________________________________________________*/
        tagVector3 vDescartes;

        vDescartes.x = this->x - _float(WINCX) / 2.f;
        vDescartes.y = -this->y + _float(WINCY) / 2.f;
        vDescartes.z = 0.f;

        return vDescartes;
    }

    tagVector3 Convert_ProjectionToScreen(const _matrix& matView, const _matrix& matProj, const D3D12_VIEWPORT& vp)
    {
        /*____________________________________________________________________
        3D 투영 좌표 -> 2D 스크린 좌표로 변환.
        ______________________________________________________________________*/
        // 1. 월드 좌표 -> 뷰 스페이스 변환.
        tagVector3 vPos_WV;
        vPos_WV.TransformCoord(*this, matView);

        // 2. 뷰 스페이스 -> 투영 변환.
        tagVector3 vPos_WVP;
        vPos_WVP.TransformCoord(vPos_WV, matProj);

        // 3. 투영 변환 -> 뷰포트 변환. (Screen 좌표 변환)
        tagVector3 vScreen;

        vScreen.x = vPos_WVP.x * (vp.Width / 2) + (vp.TopLeftX + vp.Width / 2);
        vScreen.y = vPos_WVP.y * (-1.f * vp.Height / 2) + (vp.TopLeftY + vp.Height / 2);
        vScreen.z = 0.f;

        return vScreen;
    }

    void Print() { cout << "x : " << this->x << "\t y : " << this->y << "\t z : " << this->z << endl; }

}_vec3, _rgb;

/*____________________________________________________________________
[ VECTOR4 ]
______________________________________________________________________*/
typedef struct tagVector4 : public XMFLOAT4
{
    explicit tagVector4() : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
    explicit tagVector4(const float& x, const float& y, const float& z, const float& w) : XMFLOAT4(x, y, z, w) {}
    explicit tagVector4(const float& input) : XMFLOAT4(input, input, input, input) {}
    explicit tagVector4(const XMFLOAT4& input) : XMFLOAT4(input) {}

    /*____________________________________________________________________
    연산자 오버로딩
    ______________________________________________________________________*/
    // 덧셈
    tagVector4 operator+(const tagVector4& input) { return tagVector4(this->x + input.x, this->y + input.y, this->z + input.z, this->w + input.w); }

    // 뺄셈
    tagVector4 operator-(const tagVector4& input) { return tagVector4(this->x - input.x, this->y - input.y, this->z - input.z, this->w - input.w); }

    // 곱셈
    tagVector4 operator*(const tagVector4& input) { return tagVector4(this->x * input.x, this->y * input.y, this->z * input.z, this->w * input.w); }
    tagVector4 operator*(const int& input) { return tagVector4((float)input * this->x, (float)input * this->y, (float)input * this->z, (float)input * this->w); }
    tagVector4 operator*(const float& input) { return tagVector4(input * this->x, input * this->y, input * this->z, input * this->w); }
    tagVector4 operator*(const double& input) { return tagVector4((float)input * this->x, (float)input * this->y, (float)input * this->z, (float)input * this->w); }
    friend tagVector4 operator*(const int& input, tagVector4& v1) { return tagVector4((float)input * v1.x, (float)input * v1.y, (float)input * v1.z, (float)input * v1.w); }
    friend tagVector4 operator*(const float& input, tagVector4& v1) { return tagVector4(input * v1.x, input * v1.y, input * v1.z, input * v1.w); }
    friend tagVector4 operator*(const double& input, tagVector4& v1) { return tagVector4((float)input * v1.x, (float)input * v1.y, (float)input * v1.z, (float)input * v1.w); }

    // 축약 연산자
    tagVector4 operator+=(const tagVector4& input) { return (*this) = (*this) + input; }
    tagVector4 operator-=(const tagVector4& input) { return (*this) = (*this) - input; }
    tagVector4 operator*=(const tagVector4& input) { return (*this) = (*this) * input; }
    tagVector4 operator*=(const int& input) { return (*this) = (*this) * input; }
    tagVector4 operator*=(const float& input) { return (*this) = (*this) * input; }
    tagVector4 operator*=(const double& input) { return (*this) = (*this) * input; }

    // 비교 연산자
    _bool operator==(const tagVector4& input)
    {
        XMVECTOR dst = XMVectorSet(input.x, input.y, input.z, input.w);
        XMVECTOR src = XMVectorSet(this->x, this->y, this->z, this->w);
        return XMVector4Equal(dst, src);
    }

    _bool operator!=(const tagVector4& input)
    {
        XMVECTOR dst = XMVectorSet(input.x, input.y, input.z, input.w);
        XMVECTOR src = XMVectorSet(this->x, this->y, this->z, this->w);
        return !XMVector4Equal(dst, src);
    }

    /*____________________________________________________________________
    Function.
    ______________________________________________________________________*/
    XMVECTOR Get_XMVECTOR()
    {
        return XMVectorSet(this->x, this->y, this->z, this->w);
    }

    void Normalize()
    {
        XMVECTOR temp = XMVectorSet(this->x, this->y, this->z, this->w);
        temp = XMVector4Normalize(temp);

        this->x = XMVectorGetX(temp);
        this->y = XMVectorGetY(temp);
        this->z = XMVectorGetZ(temp);
        this->w = XMVectorGetW(temp);
    }

    float Get_Length()
    {
        XMVECTOR temp = XMVectorSet(this->x, this->y, this->z, this->w);
        temp = XMVector4Length(temp);

        return XMVectorGetX(temp);
    }

    float Get_Distance(const tagVector4& vDst)
    {
        XMVECTOR dst = XMVectorSet(vDst.x, vDst.y, vDst.z, vDst.w);
        XMVECTOR src = XMVectorSet(this->x, this->y, this->z, this->w);

        XMVECTOR result = dst - src;
        result = XMVector4Length(result);

        return XMVectorGetX(result);
    }

    float Get_Angle(const tagVector4& vDst)
    {
        XMVECTOR v1 = XMVectorSet(this->x, this->y, this->z, this->w);
        XMVECTOR v2 = XMVectorSet(vDst.x, vDst.y, vDst.z, vDst.w);

        XMVECTOR angle = XMVector4AngleBetweenVectors(v1, v2);
        float   angleRadians = XMVectorGetX(angle);

        return XMConvertToDegrees(angleRadians);
    }

    float Dot(const tagVector4& vDst)
    {
        XMVECTOR dst = XMVectorSet(vDst.x, vDst.y, vDst.z, vDst.w);
        XMVECTOR src = XMVectorSet(this->x, this->y, this->z, this->w);
        XMVECTOR result = XMVector4Dot(dst, src);
        return XMVectorGetX(result);
    }

    tagVector4 Cross_Input_V1_V2(const tagVector4& v1, const tagVector4& v2)
    {
        XMVECTOR V1 = XMVectorSet(v1.x, v1.y, v1.z, v1.w);
        XMVECTOR V2 = XMVectorSet(v2.x, v2.y, v2.z, v2.w);
        XMVECTOR V3 = XMVectorSet(this->x, this->y, this->z, this->w);
        XMVECTOR result = XMVector4Cross(V1, V2, V3);

        return tagVector4(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result), XMVectorGetW(result));
    }

    tagVector4 Cross_Input_V2_V3(const tagVector4& v2, const tagVector4& v3)
    {
        XMVECTOR V1 = XMVectorSet(this->x, this->y, this->z, this->w);
        XMVECTOR V2 = XMVectorSet(v2.x, v2.y, v2.z, v2.w);
        XMVECTOR V3 = XMVectorSet(v3.x, v3.y, v3.z, v3.w);
        XMVECTOR result = XMVector4Cross(V1, V2, V3);

        return tagVector4(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result), XMVectorGetW(result));
    }

    tagVector4 Cross_Input_V1_V3(const tagVector4& v1, const tagVector4& v3)
    {
        XMVECTOR V1 = XMVectorSet(v1.x, v1.y, v1.z, v1.w);
        XMVECTOR V2 = XMVectorSet(this->x, this->y, this->z, this->w);
        XMVECTOR V3 = XMVectorSet(v3.x, v3.y, v3.z, v3.w);
        XMVECTOR result = XMVector4Cross(V1, V2, V3);

        return tagVector4(XMVectorGetX(result), XMVectorGetY(result), XMVectorGetZ(result), XMVectorGetW(result));
    }

    void Print() { cout << "x : " << this->x << "\t y : " << this->y << "\t z : " << this->z << "\t w : " << this->w << endl; }

}_vec4, _rgba;

/*____________________________________________________________________
[ CAMERA INFO ]
______________________________________________________________________*/
typedef struct tagCameraInfo
{
    tagCameraInfo()
    {
        vEye = _vec3(0.f, 0.f, 0.f);
        vAt = _vec3(0.f, 0.f, 0.f);
        vUp = _vec3(0.f, 0.f, 0.f);
        matView = XMMatrixIdentity();
    };
    tagCameraInfo(_vec3 eye, _vec3 at, _vec3 up)
        : vEye(eye), vAt(at), vUp(up)
    {
        matView = XMMatrixIdentity();
    }
    tagCameraInfo(const tagCameraInfo& input)
        : vEye(input.vEye), vAt(input.vAt), vUp(input.vUp), matView(input.matView)
    {
    }

    _vec3   vEye;
    _vec3   vAt;
    _vec3   vUp;

    _matrix   matView;

}CAMERA_INFO;

typedef struct tagPerspectiveInfo
{
    tagPerspectiveInfo()
    {
        fFovY = 0.f;
        fAspect = 0.f;
        fNearZ = 0.f;
        fFarZ = 0.f;
        matProj = XMMatrixIdentity();
    }
    tagPerspectiveInfo(_float _fovY, _float _aspect, _float _near = 0.1f, _float _far = 500.f)
        : fFovY(_fovY), fAspect(_aspect), fNearZ(_near), fFarZ(_far)
    {
        matProj = XMMatrixIdentity();
    }
    tagPerspectiveInfo(const tagPerspectiveInfo& input)
        : fFovY(input.fFovY), fAspect(input.fAspect), fNearZ(fNearZ), fFarZ(input.fFarZ), matProj(input.matProj)
    {
    }

    _float fFovY;
    _float fAspect;
    _float fNearZ;
    _float fFarZ;

    _matrix   matProj;

}PROJ_INFO;

typedef struct tagOrthographicInfo
{
    tagOrthographicInfo()
    {
        fWidth = 0.f;
        fHeight = 0.f;
        fNearZ = 0.f;
        fFarZ = 1.f;
        matProj = XMMatrixIdentity();
    }
    tagOrthographicInfo(_float _width, _float _height, _float _near = 0.f, _float _far = 1.f)
        : fWidth(_width), fHeight(_height), fNearZ(_near), fFarZ(_far)
    {
        matProj = XMMatrixIdentity();
    }
    tagOrthographicInfo(const tagOrthographicInfo& input)
        : fWidth(input.fWidth), fHeight(input.fHeight), fNearZ(input.fNearZ), fFarZ(input.fFarZ), matProj(input.matProj)
    {
    }

    _float fWidth;
    _float fHeight;
    _float fNearZ;
    _float fFarZ;

    _matrix matProj;

}ORTHO_INFO;

/*____________________________________________________________________
[ CB Struct ]
______________________________________________________________________*/
typedef struct tagCBMatrixInfo
{
    XMFLOAT4X4 matWorld;
    XMFLOAT4X4 matView;
    XMFLOAT4X4 matProj;
    XMFLOAT4X4 matWVP;

}CB_MATRIX_INFO;//오브젝트
typedef struct tagCBBoneInfo
{
    XMFLOAT4X4 matbone[80];
}CB_BONE_INFO;//다이나믹매쉬

typedef struct tagCBDownSample
{
    XMFLOAT4 DownSampleOffset[16];

}CB_SAMPLE_INFO;//다운샘플
typedef struct tagCBPIXELSample
{
    XMFLOAT2 PixelKerneIH[13];
    XMFLOAT2 PixelKernelV[13];

}CB_PIXEL_INFO;//다운샘플

typedef struct tagCBBlurSample
{
    float BlurWeight[13];

}CB_BLUR_INFO;//블러


typedef struct tagDistionInfo
{
    XMFLOAT4 DistorScale;

}CB_DISTORT_INFO;

typedef struct tagCBTextureInfo
{
    XMFLOAT4 vecTexInfo; //r 스펙 g 이메 

}CB_TEXTURE_INFO;

typedef struct tagCBShadowInfo
{
    XMFLOAT4X4 matWorld;
    XMFLOAT4X4 matView;
    XMFLOAT4X4 matProj;
    bool blsMesh;
}CB_SHADOW_INFO;//세도우 댑스용

typedef struct tagCBLIGHTMATRIXInfo
{
    XMFLOAT4X4 matLightView;
    XMFLOAT4X4 matLightProj;
}CB_LIGHTMATRIX_INFO;//그림자 용

typedef struct tagCBINVERSEInfo
{
    XMFLOAT4X4 matViewInv;
    XMFLOAT4X4 matProjInv;
    XMFLOAT4   vCamPos;
}CB_INVERSEMATRIX_INFO;


typedef struct tagCBLightInfo
{
    XMFLOAT4  g_vLightDir;
    XMFLOAT4  g_vLightDiffuse;
    XMFLOAT4  g_vLightAmibient;
    XMFLOAT4  g_vLightSpecular;
    XMFLOAT4  g_vLightPos;
    float     g_fRange;
}CB_LIGHT_INFO;//조명용


typedef struct BoneInfo
{
    _matrix matboneOffset;
    _matrix matfinalTransform;
}BONE_INFO;

typedef struct tagSubmeshGeometry
{
    _uint   uiIndexCount = 0;
    _uint   uiStartIndexLocation = 0;
    _int   iBaseVertexLocation = 0;

    // Bounding box of the geometry defined by this submesh. 
    // This is used in later chapters of the book.
    BoundingBox Bounds;

}SUBMESH_GEOMETRY;



namespace Engine
{
    typedef struct tagVertexColor
    {
        tagVertexColor() {}
        tagVertexColor(const _vec3& pos, const _rgba& color)
            : vPos(pos), vColor(color)
        {}

        _vec3      vPos;
        _rgba      vColor;

    }VTXCOL;

    //const DWORD      VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

    typedef struct tagVertpexTexture
    {
        _vec3         vPos;
        _vec3         vNormal;
        _vec2         vTexUV;

    }VTXTEX;

    typedef struct tagVertpexTextureNONOMAL
    {
        tagVertpexTextureNONOMAL() {}
        tagVertpexTextureNONOMAL(const _vec3& pos, const _vec2& uv)
            : vPos(pos), vTexUV(uv)
        {}


        _vec3         vPos;
        _vec2         vTexUV;

    }VTXRC;


    //const DWORD      VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

    //typedef   struct tagVertexCube
    //{
    //   D3DXVECTOR3      vPos;
    //   D3DXVECTOR3      vTex;

    //}VTXCUBE;

    //const DWORD      VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

    typedef   struct   tagIndex16
    {
        unsigned short      _1, _2, _3;

    }INDEX16;

    typedef   struct   tagIndex32
    {
        unsigned long      _1, _2, _3;

    }INDEX32;

    typedef struct tagVerMESH
    {
        _vec3 Pos;
        _vec3 Normal;
        _vec2 TexCoord;
        _vec4 BoneIds[2];
        _vec4 BoneWeights[2];

    }VTXMESH;


}

enum class ROOT_SIG_TYPE
{
    INPUT_OBJECT,
    INPUT_TEXTURE,
    INPUT_MESH,
    INPUT_TERRAIN,
    INPUT_LIGHT,
    INPUT_BLEND,
    INPUT_SHADOWDEPTH,
    INPUT_DOWNSAMPLE,
    INPUT_SSAO,
    INPUT_BLUR,
    INPUT_DISTORT,
    ENDSIG,
};

struct MeshEntry {
    MeshEntry()
    {
        NumIndices = 0;
        BaseVertex = 0;
        BaseIndex = 0;
        MaterialIndex = 0xFFFFFFFF;
        m_blsTexture = false;
    }

    unsigned int NumIndices;
    unsigned int BaseVertex;
    unsigned int BaseIndex;
    unsigned int MaterialIndex;
    bool m_blsTexture;
};

typedef enum _D3DLIGHTTYPE {
    D3DLIGHT_POINT = 1,
    D3DLIGHT_SPOT = 2,
    D3DLIGHT_DIRECTIONAL = 3,
    D3DLIGHT_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
} LIGHTTYPE;


typedef struct _D3DLIGHT {
    LIGHTTYPE m_eType;
    _vec4 m_vDiffuse;
    _vec4 m_vSpecular;
    _vec4 m_vAmbient;
    _vec4 m_vPosition;
    _vec4 m_vDirection;
    float m_fRange;
}D3DLIGHT;


#endif // Engine_Struct_h__