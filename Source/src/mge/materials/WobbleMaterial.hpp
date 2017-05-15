#ifndef WobbleMaterial_H
#define WobbleMaterial_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"


class WobbleMaterial : public AbstractMaterial
{
public:
	WobbleMaterial(Texture* pDiffuseTexture);
	virtual ~WobbleMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);

	float speed = 1;
	float intensity = 0.15;

protected:
private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	Texture* _diffuseTexture;

	WobbleMaterial(const WobbleMaterial&);
	WobbleMaterial& operator=(const WobbleMaterial&);

	float time = 0;

};

#endif // WobbleMaterial_H
