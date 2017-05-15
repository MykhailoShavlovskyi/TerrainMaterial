#ifndef TERRAINMATERIAL_H
#define TERRAINMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"

/**
 * Simple single texture material, this is a sample which doesn't cache anything upfront and
 * passes in separate matrices for the MVP calculation
 */
class TerrainMaterial : public AbstractMaterial
{
    public:
		TerrainMaterial(Texture* pHeightMap, Texture* pSplatMap, std::vector<Texture*> diffuseTextureBlock, Camera * pCamera);
        virtual ~TerrainMaterial();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

		void setSpecularColor(glm::vec3 color);
		void setDiffuseColor(glm::vec3 color);
		void setShininess(int value);
		void setIntensity(float value);
		void setWaveParameters(std::vector<float> values);

		bool debugMode = false;

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        Texture* _heightMap;
		Texture* _splatMap;
		Texture* _waveDebugSplatMap;

		Texture* _diffuse1;
		Texture* _diffuse2;
		Texture* _diffuse3;
		Texture* _diffuse4;

		float _time = 0;

		float _waveRadiusMaxOffset;
		float _waveRadiusMaxOffsetFrequency;
		float _waveWidth;
		float _waveMaxHeight;
		float _waveMaxHeightOffset;
		float _waveHeightOffsetFrequency;
		float _waveSpeed;
		float _waveCount;

		

		glm::vec3 _diffuseColor = glm::vec3(1, 1, 1);
		glm::vec3 _specularColor = glm::vec3(1, 1, 1);

		int _shininess = 2;

		Camera * _camera;
		 
		float _intensity = 1;

		TerrainMaterial(const TerrainMaterial&);
		TerrainMaterial& operator=(const TerrainMaterial&);

};

#endif // TERRAINMATERIAL_H
