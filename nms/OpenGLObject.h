#pragma once

class OpenGLObject
{
public:
	virtual void use() = 0;
	virtual void cleanup() = 0;
	virtual void configure() = 0;
};