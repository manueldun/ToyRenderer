#include"ConvertToMeshes.h"
#include<algorithm>
#include<iostream>
#include<string>
#include"Layout.h"
#include"PBRMaterial.h"
#include"Material.h"
std::vector<Mesh> convertToMeshes(const std::vector<Utils::OBJData>& objData, const std::vector<std::shared_ptr<Shader>>& shaders)
{
	std::vector<Mesh> outputMeshes;
	for (const auto& obj : objData)
	{

		std::vector<SubMesh> subMeshes;

		for (const auto& subOBJ : obj.getSubOBJDatas())
		{
			std::shared_ptr<PhongMaterial> material;
			if (subOBJ.getOBJMaterial()->m_diffuseMapPath.length() == 0 && (subOBJ.getOBJMaterial()->m_bumpMapPath.length() == 0))
			{
				material = std::shared_ptr<PhongMaterial>(new PhongMaterial(
					subOBJ.getOBJMaterial()->m_diffuseRed,
					subOBJ.getOBJMaterial()->m_diffuseGreen,
					subOBJ.getOBJMaterial()->m_diffuseBlue,
					subOBJ.getOBJMaterial()->m_specularRed,
					subOBJ.getOBJMaterial()->m_specularGreen,
					subOBJ.getOBJMaterial()->m_specularBlue,
					subOBJ.getOBJMaterial()->m_shininess,
					shaders.at(0)
				));
			}
			else if (subOBJ.getOBJMaterial()->m_diffuseMapPath.length() != 0 && (subOBJ.getOBJMaterial()->m_bumpMapPath.length() == 0))
			{
				material = std::shared_ptr<PhongMaterial>(new PhongMaterial(
					subOBJ.getOBJMaterial()->m_specularRed,
					subOBJ.getOBJMaterial()->m_specularGreen,
					subOBJ.getOBJMaterial()->m_specularBlue,
					subOBJ.getOBJMaterial()->m_shininess,
					std::shared_ptr<Texture>(new Texture(subOBJ.getOBJMaterial()->m_diffuseMapPath)),
					shaders.at(1)
				));
			}
			else if (subOBJ.getOBJMaterial()->m_diffuseMapPath.length() == 0 && (subOBJ.getOBJMaterial()->m_bumpMapPath.length() != 0))
			{
				material = std::shared_ptr<PhongMaterial>(new PhongMaterial(
					subOBJ.getOBJMaterial()->m_diffuseRed,
					subOBJ.getOBJMaterial()->m_diffuseGreen,
					subOBJ.getOBJMaterial()->m_diffuseBlue,
					subOBJ.getOBJMaterial()->m_specularRed,
					subOBJ.getOBJMaterial()->m_specularGreen,
					subOBJ.getOBJMaterial()->m_specularBlue,
					subOBJ.getOBJMaterial()->m_shininess,
					std::shared_ptr<Texture>(new Texture(subOBJ.getOBJMaterial()->m_bumpMapPath)),
					shaders.at(2)
				));
			}
			else if (subOBJ.getOBJMaterial()->m_diffuseMapPath.length() != 0 && (subOBJ.getOBJMaterial()->m_bumpMapPath.length() != 0)) {

				material = std::shared_ptr<PhongMaterial>(new PhongMaterial(
					subOBJ.getOBJMaterial()->m_specularRed,
					subOBJ.getOBJMaterial()->m_specularGreen,
					subOBJ.getOBJMaterial()->m_specularBlue,
					subOBJ.getOBJMaterial()->m_shininess,
					std::shared_ptr<Texture>(new Texture(subOBJ.getOBJMaterial()->m_diffuseMapPath)),
					std::shared_ptr<Texture>(new Texture(subOBJ.getOBJMaterial()->m_bumpMapPath)),
					shaders.at(3)
				));
			}
			std::shared_ptr<IndexBuffer> indexBuffer =
				std::make_shared<IndexBuffer>(
					subOBJ.getIndexBuffer().data(),
					subOBJ.getIndexBuffer().size());
			subMeshes.push_back(SubMesh(indexBuffer, material));

		}

		outputMeshes.push_back(Mesh(subMeshes, obj.getVertexBuffer()));


	}

	return outputMeshes;
}

std::vector<Mesh> convertToMeshes(const tinygltf::Model& model, const std::shared_ptr<Shader>& shader)
{
	std::vector<Mesh> meshBuffers = separateIndicesFromVertexData(model, shader);

	return meshBuffers;
}

std::vector<float> convertDataToFloatBuffer(
	const std::vector<unsigned char>& data,
	size_t offset,
	size_t numOfElements)
{
	return std::vector<float>(
		reinterpret_cast<const float*>(&data[offset]),
		reinterpret_cast<const float*>(&data[offset]) + numOfElements);
}

std::vector<unsigned short> convertDataToUIntBuffer(
	const std::vector<unsigned char>& data,
	size_t offset,
	size_t numOfElements)
{

	return std::vector<unsigned short>(
		reinterpret_cast<const unsigned short*>(&data[offset]),
		reinterpret_cast<const unsigned short*>(&data[offset]) + numOfElements);
}

std::vector<Mesh> separateIndicesFromVertexData(const tinygltf::Model& model, std::shared_ptr<Shader> shader)
{
	std::vector<Mesh> meshes;
	unsigned int currentOffset = 0;


	for (const auto& mesh : model.meshes)
	{

		for (const auto& primitive : mesh.primitives)
		{

			std::vector<float> positionBuffer;
			std::vector<float> texCoordBuffer;
			std::vector<float> normalBuffer;
			std::vector<float> tangentBuffer;

			bool hasTangent = true;
			tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
			const std::size_t indexAccessorCount = indexAccessor.count;
			unsigned int indexAccessorAttribAmount = 0;
			unsigned int indexAccessorAttribComponentAmount = 0;
			unsigned int indexAccessorType = indexAccessor.type;
			unsigned int indexAccessorComponentType = indexAccessor.componentType;
			switch (indexAccessorType)
			{
			case TINYGLTF_TYPE_VEC2:
				indexAccessorAttribAmount = 2;
				break;
			case TINYGLTF_TYPE_VEC3:
				indexAccessorAttribAmount = 3;
				break;
			case TINYGLTF_TYPE_SCALAR:
				indexAccessorAttribAmount = 1;
				break;
			default:
				throw std::runtime_error("Attribute type not implemented");
			}
			switch (indexAccessorComponentType)
			{
			case TINYGLTF_COMPONENT_TYPE_FLOAT:
				indexAccessorAttribComponentAmount = 4;
				break;
			case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
				indexAccessorAttribComponentAmount = 2;
				break;
			case TINYGLTF_COMPONENT_TYPE_INT:
				indexAccessorAttribComponentAmount = 4;
				break;
			default:
				throw std::runtime_error("Attribute type not implemented(gltf loading)");
			}

			const std::size_t byteOffsetAccessor = indexAccessor.byteOffset;
			tinygltf::BufferView bufferView = model.bufferViews[indexAccessor.bufferView];
			const std::size_t byteLength = bufferView.byteLength;
			const std::size_t byteOffsetBufferView = bufferView.byteOffset;
			tinygltf::Buffer rawIndexBuffer = model.buffers[bufferView.buffer];

			std::size_t elementOffset = (byteOffsetAccessor + byteOffsetBufferView);

			std::vector<unsigned short> outputIndexBuffer =
				convertDataToUIntBuffer(
					rawIndexBuffer.data,
					elementOffset,
					indexAccessorCount);


			const unsigned int floatByteLength = 4;
			const std::string attributes[] = { "POSITION","TEXCOORD_0","NORMAL","TANGENT" };
			for (const auto& attributeName : attributes)
			{
				try {
					tinygltf::Accessor attributeAccessor = model.accessors[primitive.attributes.at(attributeName)];
					tinygltf::BufferView attributeAccessorBufferView = model.bufferViews[attributeAccessor.bufferView];
					tinygltf::Buffer attributeAccessorGltfBuffer = model.buffers[attributeAccessorBufferView.buffer];
					unsigned int accessorAttribAmount = 0;
					unsigned int accessorAttribComponentAmount = 0;
					unsigned int accessorType = attributeAccessor.type;
					unsigned int accessorComponentType = attributeAccessor.componentType;
					switch (accessorType)
					{
					case TINYGLTF_TYPE_VEC2:
						accessorAttribAmount = 2;
						break;
					case TINYGLTF_TYPE_VEC3:
						accessorAttribAmount = 3;
						break;
					case TINYGLTF_TYPE_VEC4:
						accessorAttribAmount = 4;
						break;
					case TINYGLTF_TYPE_SCALAR:
						accessorAttribAmount = 1;
						break;
					default:
						throw std::runtime_error("Attribute type not implemented");
					}
					switch (accessorComponentType)
					{
					case TINYGLTF_COMPONENT_TYPE_FLOAT:
						accessorAttribComponentAmount = 4;
						break;
					case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
						accessorAttribComponentAmount = 2;
						break;
					case TINYGLTF_COMPONENT_TYPE_INT:
						accessorAttribComponentAmount = 4;
						break;
					default:
						throw std::runtime_error("Attribute type not implemented");
					}

					const size_t accessorCount = attributeAccessor.count;
					const size_t attributeByteLength = attributeAccessorBufferView.byteLength;
					const size_t attributeByteOffsetAccessor = attributeAccessor.byteOffset;
					const size_t attributeByteOffsetBufferView = attributeAccessorBufferView.byteOffset;
					std::size_t attributeOffset = (static_cast<std::size_t>(attributeByteOffsetAccessor) + static_cast<std::size_t>(attributeByteOffsetBufferView));



					std::vector<float> vertexBuffer =
						convertDataToFloatBuffer(
							attributeAccessorGltfBuffer.data,
							attributeOffset,
							accessorCount * accessorAttribAmount);
					if (attributeName.compare("POSITION") == 0)
					{
						positionBuffer.insert(std::end(positionBuffer), std::begin(vertexBuffer), std::end(vertexBuffer));
					}
					else if (attributeName.compare("TEXCOORD_0") == 0)
					{
						texCoordBuffer.insert(std::end(texCoordBuffer), std::begin(vertexBuffer), std::end(vertexBuffer));
					}
					else if (attributeName.compare("NORMAL") == 0)
					{
						normalBuffer.insert(std::end(normalBuffer), std::begin(vertexBuffer), std::end(vertexBuffer));
					}
					else if (attributeName.compare("TANGENT") == 0)
					{
						tangentBuffer.insert(std::end(tangentBuffer), std::begin(vertexBuffer), std::end(vertexBuffer));
					}


				}
				catch (std::out_of_range oorex)
				{
					tangentBuffer = std::vector<float>((positionBuffer.size() / 3) * 4, 0.0f);
					hasTangent = false;
				}
			}


			tinygltf::Material material = model.materials[primitive.material];
			const float albedo[] = {
				static_cast<float>(material.pbrMetallicRoughness.baseColorFactor.at(0)),
				static_cast<float>(material.pbrMetallicRoughness.baseColorFactor.at(1)),
				static_cast<float>(material.pbrMetallicRoughness.baseColorFactor.at(2)),
			};


			float roughnessConstant = static_cast<float>(material.pbrMetallicRoughness.roughnessFactor);
			float metallicConstant = static_cast<float>(material.pbrMetallicRoughness.metallicFactor);
			std::shared_ptr<Texture> albedoTextureGPU = std::make_shared<Texture>(1.0f, 1.0f, 1.0f);
			tinygltf::PbrMetallicRoughness pbrMetallicRoughness = material.pbrMetallicRoughness;
			if (pbrMetallicRoughness.baseColorTexture.index != -1)
			{
				tinygltf::Texture albedoTexture = model.textures[pbrMetallicRoughness.baseColorTexture.index];
				tinygltf::Image albedoImage = model.images[albedoTexture.source];

				albedoTextureGPU =
					std::make_shared<Texture>(
						albedoImage.width,
						albedoImage.height,
						albedoImage.component,
						albedoImage.image);
			}


			std::shared_ptr<Texture> metallicRoughnessTextureGPU = std::make_shared<Texture>(1.0f, 1.0f, 1.0f);

			if (pbrMetallicRoughness.metallicRoughnessTexture.index != -1)
			{
				tinygltf::Texture metallicRoughnessTexture = model.textures[pbrMetallicRoughness.metallicRoughnessTexture.index];
				tinygltf::Image metallicRoughnessImage = model.images[metallicRoughnessTexture.source];


				metallicRoughnessTextureGPU =
					std::make_shared<Texture>(
						metallicRoughnessImage.width,
						metallicRoughnessImage.height,
						metallicRoughnessImage.component,
						metallicRoughnessImage.image);
			}

			std::shared_ptr<Texture> normalTextureGPU = std::make_shared<Texture>(0.0f, 0.0f, 1.0f);//default normal direction

			if (hasTangent)
			{
				tinygltf::Texture normalTexture = model.textures[material.normalTexture.index];
				tinygltf::Image normalImage = model.images[normalTexture.source];

				normalTextureGPU =
					std::make_shared<Texture>(
						normalImage.width,
						normalImage.height,
						normalImage.component,
						normalImage.image);
			}
			std::shared_ptr<PBRMaterial> pbrMaterial =
				std::make_shared<PBRMaterial>(
					albedo[0],
					albedo[1],
					albedo[2],
					roughnessConstant,
					metallicConstant,
					albedoTextureGPU,
					normalTextureGPU,
					metallicRoughnessTextureGPU,
					shader,
					hasTangent
					);
			std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
			std::shared_ptr<IndexBuffer> indexBuffer =
				std::make_shared<IndexBuffer>(
					outputIndexBuffer.data(),
					outputIndexBuffer.size(),
					2);

			std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers =
			{
				std::make_shared<VertexBuffer>(positionBuffer.data(),positionBuffer.size() ,0,3),
				std::make_shared<VertexBuffer>(texCoordBuffer.data(),texCoordBuffer.size() ,1,2),
				std::make_shared<VertexBuffer>(normalBuffer.data(),normalBuffer.size() ,2,3),
				std::make_shared<VertexBuffer>(tangentBuffer.data(),tangentBuffer.size() ,3,4)
			};
			vertexArray->setVertexBuffers(vertexBuffers);//to avoid vertexBuffer destruction


			Mesh mesh({ SubMesh(indexBuffer, pbrMaterial) }, vertexArray);
			meshes.push_back(mesh);
		}
	}

	return meshes;
}
