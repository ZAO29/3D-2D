#pragma once

#include <ZGL/Texture.h>

class LoadableTexture : public Texture
{
public:
	LoadableTexture();
	~LoadableTexture();


	virtual bool Load(std::string file) override;
};

