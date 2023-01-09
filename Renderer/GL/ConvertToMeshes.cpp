#include"ConvertToMeshes.h"
#include"SubMesh.h"
#include"Material.h"
#include"PBRMaterial.h"
//deprecated
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
			//deprecated
			//subMeshes.push_back(SubMesh(indexBuffer, material));

		}
		//deprecated
		//outputMeshes.push_back(Mesh(subMeshes, obj.getVertexBuffer()));


	}

	return outputMeshes;
}

std::vector<Mesh> convertToMeshes(const tinygltf::Model& model)
{
	std::vector<Mesh> meshBuffers = separateIndicesFromVertexData(model);

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
std::vector<Mesh> separateIndicesFromVertexData(const tinygltf::Model& model)
{
	std::vector<Mesh> meshes;


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
						positionBuffer = std::move(vertexBuffer);
					}
					else if (attributeName.compare("TEXCOORD_0") == 0)
					{
						texCoordBuffer = std::move(vertexBuffer);
					}
					else if (attributeName.compare("NORMAL") == 0)
					{
						normalBuffer = std::move(vertexBuffer);
					}
					else if (attributeName.compare("TANGENT") == 0)
					{
						tangentBuffer = std::move(vertexBuffer);
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
			RawTexture albedoRawTexture({ 255,255,255 }, 1, 1, 3);
			tinygltf::PbrMetallicRoughness pbrMetallicRoughness = material.pbrMetallicRoughness;
			if (pbrMetallicRoughness.baseColorTexture.index != -1)
			{
				tinygltf::Texture albedoTexture = model.textures[pbrMetallicRoughness.baseColorTexture.index];
				tinygltf::Image albedoImage = model.images[albedoTexture.source];

				albedoRawTexture =
					RawTexture(
						albedoImage.image,
						albedoImage.width,
						albedoImage.height,
						albedoImage.component
					);
			}


			RawTexture metallicRoughnessRawTexture({ 255,255,255 }, 1, 1, 3);
			if (pbrMetallicRoughness.metallicRoughnessTexture.index != -1)
			{
				tinygltf::Texture metallicRoughnessTexture = model.textures[pbrMetallicRoughness.metallicRoughnessTexture.index];
				tinygltf::Image metallicRoughnessImage = model.images[metallicRoughnessTexture.source];


				metallicRoughnessRawTexture =
					RawTexture(
						metallicRoughnessImage.image,
						metallicRoughnessImage.width,
						metallicRoughnessImage.height,
						metallicRoughnessImage.component
					);
			}

			RawTexture normalRawTexture({
				128,
				128,
				255 }, 1, 1, 3);//default normal direction

			if (hasTangent)
			{
				tinygltf::Texture normalTexture = model.textures[material.normalTexture.index];
				tinygltf::Image normalImage = model.images[normalTexture.source];

				normalRawTexture =
					RawTexture(
						normalImage.image,
						normalImage.width,
						normalImage.height,
						normalImage.component);
			}
			PBRMaterial pbrMaterial =
				PBRMaterial(
					albedo[0],
					albedo[1],
					albedo[2],
					roughnessConstant,
					metallicConstant,
					albedoRawTexture,
					normalRawTexture,
					metallicRoughnessRawTexture,
					hasTangent
				);

			//Mesh outMesh({ SubMesh(outputIndexBuffer, pbrMaterial) }, {
			//	positionBuffer,
			//	texCoordBuffer,
			//	normalBuffer,
			//	tangentBuffer
			//	});
			meshes.push_back(Mesh({ SubMesh(outputIndexBuffer, pbrMaterial) }, {
					positionBuffer,
					texCoordBuffer,
					normalBuffer,
					tangentBuffer
					}));
			return meshes;
		}
	}
	return meshes;
}
