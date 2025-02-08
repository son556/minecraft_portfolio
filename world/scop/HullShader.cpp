#include "pch.h"
#include "HullShader.h"
#include "EXEPath.h"

HullShader::HullShader(
	ComPtr<ID3D11Device> device, 
	wstring path, 
	string const& entry_point, 
	string const& version
)
{
	path = EXEPath::ori_path + path;
	const uint32 compileFlag = D3DCOMPILE_DEBUG |
		D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry_point.c_str(),
		version.c_str(),
		compileFlag,
		0,
		this->blob.GetAddressOf(),
		nullptr
	);
	CHECK(hr);
	hr = device->CreateHullShader(
		this->blob->GetBufferPointer(),
		this->blob->GetBufferSize(),
		nullptr,
		this->hull_shader.GetAddressOf()
	);
	CHECK(hr);
}

HullShader::~HullShader()
{
}

ComPtr<ID3D11HullShader> HullShader::getComPtr() const
{
	return this->hull_shader;
}

ComPtr<ID3DBlob> HullShader::getBlob() const
{
	return this->blob;
}
