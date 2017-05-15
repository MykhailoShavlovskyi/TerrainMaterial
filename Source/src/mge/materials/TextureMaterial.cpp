#include <glm.hpp>

#include "mge/materials/TerrainMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/util/LightUtil.hpp"
#include <SFML/Window/Keyboard.hpp>

#include "mge/config.hpp"

ShaderProgram* TerrainMaterial::_shader = NULL;

TerrainMaterial::TerrainMaterial(Texture * pHeightMap, Texture* pSplatMap, std::vector<Texture*> diffuseTextureBlock, Camera * pCamera):_heightMap(pHeightMap), _splatMap(pSplatMap),
_diffuse1(diffuseTextureBlock[0]), _diffuse2(diffuseTextureBlock[1]), _diffuse3(diffuseTextureBlock[2]), _diffuse4(diffuseTextureBlock[3]), _camera(pCamera)
{
    _lazyInitializeShader();
	_waveDebugSplatMap = Texture::load(config::MGE_TEXTURE_PATH + "terrain/splatmapG.jpg");
}

TerrainMaterial::~TerrainMaterial() {}

void TerrainMaterial::_lazyInitializeShader()
{
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"terrain.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"terrain.fs");
        _shader->finalize();
    }
}

void TerrainMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_heightMap) return;
	   
	//std::cout << _time << std::endl;
	_time += _waveSpeed;
	if (_time > 10000)
	{
		_time = 0;
	}
    _shader->use();

	glUniform1i(_shader->getUniformLocation("debugMode"), debugMode);

	//pass time
	glUniform1f(_shader->getUniformLocation("time"), _time);

	//pass intensity
	glUniform1f(_shader->getUniformLocation("intensity"), _intensity);

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _heightMap->getId());
    glUniform1i (_shader->getUniformLocation("heightMap"), 0);

	//setup texture slot 1
	if (!debugMode)
	{
		glActiveTexture(GL_TEXTURE0+1);
		glBindTexture(GL_TEXTURE_2D, _splatMap->getId());
		glUniform1i(_shader->getUniformLocation("splatMap"), 1);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, _waveDebugSplatMap->getId());
		glUniform1i(_shader->getUniformLocation("splatMap"), 1);
	}

	//setup texture slot 2-5
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, _diffuse1->getId());
	glUniform1i(_shader->getUniformLocation("diffuse1"), 2);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, _diffuse2->getId());
	glUniform1i(_shader->getUniformLocation("diffuse2"), 3);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, _diffuse3->getId());
	glUniform1i(_shader->getUniformLocation("diffuse3"), 4);
	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D, _diffuse4->getId());
	glUniform1i(_shader->getUniformLocation("diffuse4"), 5);

	//send camera position
	glUniform4fv(_shader->getUniformLocation("cameraPosition"),1, glm::value_ptr(_camera->getWorldPosition()));

	//send material diffuse and specular color
	glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(_diffuseColor));
	glUniform3fv(_shader->getUniformLocation("specularColor"), 1, glm::value_ptr(_specularColor));
	glUniform1i(_shader->getUniformLocation("shininess"), _shininess);

	//send wave parameters
	glUniform1f(_shader->getUniformLocation("waveRadiusMaxOffset"), _waveRadiusMaxOffset);
	glUniform1f(_shader->getUniformLocation("waveRadiusMaxOffsetFrequency"), _waveRadiusMaxOffsetFrequency); 
	glUniform1f(_shader->getUniformLocation("waveWidth"), _waveWidth);
	glUniform1f(_shader->getUniformLocation("waveMaxHeight"), _waveMaxHeight); 
	glUniform1f(_shader->getUniformLocation("waveMaxHeightOffset"), _waveMaxHeightOffset); 
	glUniform1f(_shader->getUniformLocation("waveHeightOffsetFrequency"), _waveHeightOffsetFrequency); 
	glUniform1f(_shader->getUniformLocation("waveCount"), _waveCount);

	//send point lights data
	int pointLightCount = LightUtil::GetPointLigthsCount();
	glUniform1f(_shader->getUniformLocation("pointLightCount"), pointLightCount); //point light count
	glUniform3fv(_shader->getUniformLocation("pointLightPos"), pointLightCount, glm::value_ptr(LightUtil::GetPointLigthsPositions()[0])); //point light positiom
	glUniform3fv(_shader->getUniformLocation("pointLightColor"), pointLightCount, glm::value_ptr(LightUtil::GetPointLigthsColors()[0])); //point light color
	glUniform3fv(_shader->getUniformLocation("pointAttenuation"), pointLightCount, glm::value_ptr(LightUtil::GetPointLigthsAttenuations()[0])); //point light attenuation

	//send directional lights data
	int directionalLightCount = LightUtil::GetDirectionalLigthsCount();
	glUniform1f(_shader->getUniformLocation("directionalLightCount"), directionalLightCount); //directional light count
	glUniform3fv(_shader->getUniformLocation("directionalLightColor"), directionalLightCount, glm::value_ptr(LightUtil::GetDirectionalLigthsColors()[0])); //directional light color
	glUniform3fv(_shader->getUniformLocation("directionalLightDir"), directionalLightCount, glm::value_ptr(LightUtil::GetDirectionalLigthsDirections()[0])); //directional light direction

	//send spoty lights data																																	
	int spotLightCount = LightUtil::GetSpotLigthsCount();
	glUniform1f(_shader->getUniformLocation("spotLightCount"), spotLightCount); //spot light count
	glUniform3fv(_shader->getUniformLocation("spotLightColor"), spotLightCount, glm::value_ptr(LightUtil::GetSpotLigthsColors()[0])); //spot light color
	glUniform3fv(_shader->getUniformLocation("spotLightPos"), spotLightCount, glm::value_ptr(LightUtil::GetSpotLigthsPositions()[0])); //spot light position
	glUniform3fv(_shader->getUniformLocation("spotLightDir"), spotLightCount, glm::value_ptr(LightUtil::GetSpotLigthsDirections()[0])); //spot light direction
	glUniform3fv(_shader->getUniformLocation("spotLightAttenuation"), spotLightCount, glm::value_ptr(LightUtil::GetSpotLigthsAttenuations()[0])); //spot light attenuation
	glUniform1fv(_shader->getUniformLocation("spotLightAngle"), spotLightCount, &LightUtil::GetSpotLigthsAngles()[0]); //spot light angle
	glUniform1fv(_shader->getUniformLocation("spotFallOffRate"), spotLightCount, &LightUtil::GetSpotLigthsFallOffRates()[0]); //spot lights fall off rate

    //pass in all MVP matrices separately
    glUniformMatrix4fv ( _shader->getUniformLocation("projectionMatrix"),   1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("viewMatrix"),         1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("modelMatrix"),        1, GL_FALSE, glm::value_ptr(pModelMatrix));

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(
        _shader->getAttribLocation("vertex"),
        _shader->getAttribLocation("normal"),
        _shader->getAttribLocation("uv")
    );
}

void TerrainMaterial::setDiffuseColor(glm::vec3 color)
{
	_diffuseColor = color;
}

void TerrainMaterial::setSpecularColor(glm::vec3 color)
{
	_specularColor = color;
}

void TerrainMaterial::setShininess(int value)
{
	_shininess = value;
	if (_shininess < 1)
	{
		_shininess = 1;
	}
	else if (_shininess > 256)
	{
		_shininess = 256;
	}
}

void TerrainMaterial::setIntensity(float value)
{
	_intensity = value;
}

void TerrainMaterial::setWaveParameters(std::vector<float> values)
{
	_waveRadiusMaxOffset=values[0];
	_waveRadiusMaxOffsetFrequency = values[1];
	_waveWidth = values[2];
	_waveMaxHeight = values[3];
	_waveMaxHeightOffset = values[4];
	_waveHeightOffsetFrequency = values[5];
	_waveSpeed = values[6];
	_waveCount = values[7];
}
