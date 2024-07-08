#include "CorePch.hpp"
#include "PMXFile.hpp"

#include <common/src/Locale.hpp>
#include <common/src/Stream.hpp>

namespace mm
{
	void PMXFile::ParseText(std::ifstream& stream, std::string& string)
	{
		uint32_t length;
		Read(stream, length);
		string.resize(length);
		stream.read(&string[0], length);
		if (m_header.textEncoding == PMXFile::ENC_UTF16LE) {
			string = Locale::UTF16LEToUTF8(string.c_str(), string.length());
		}
	}

	void PMXFile::ParseHeader(std::ifstream& stream)
	{
		Read(stream, m_header.magic);
		if (memcmp(m_header.magic, "PMX ", 4))
			throw PMXParseError("Invalid file");

		Read(stream, m_header.version);
		Read(stream, m_header.dataCount);
		Read(stream, m_header.textEncoding);
		Read(stream, m_header.extraUVCount);
		if (m_header.extraUVCount > 0)
			throw PMXParseError("Extra UV is not supported");
		Read(stream, m_header.vertexIndexSize);
		Read(stream, m_header.textureIndexSize);
		Read(stream, m_header.materialIndexSize);
		Read(stream, m_header.boneIndexSize);
		Read(stream, m_header.morphIndexSize);
		Read(stream, m_header.rigidbodyIndexSize);
	}

	void PMXFile::ParseInfo(std::ifstream& stream)
	{
		ParseText(stream, m_info.nameJP);
		ParseText(stream, m_info.nameEN);
		ParseText(stream, m_info.commentJP);
		ParseText(stream, m_info.commentEN);
	}

	void PMXFile::ParseVertex(std::ifstream& stream)
	{
		uint32_t vertexCount;
		Read(stream, vertexCount);
		m_vertices.resize(vertexCount);
		for (uint32_t i = 0; i < vertexCount; ++i) {
			Vertex& vertex = m_vertices[i];
			Read(stream, vertex.position);
			Read(stream, vertex.normal);
			Read(stream, vertex.uv);

			Read(stream, vertex.blendingType);
			switch (vertex.blendingType) {
			case PMXFile::VERTEX_BDEF1:
			{
				auto& blending = vertex.blending.bdef1;
				Read(stream, blending.boneIndex, m_header.boneIndexSize);
			}
			break;
			case PMXFile::VERTEX_BDEF2:
			{
				auto& blending = vertex.blending.bdef2;
				Read(stream, blending.boneIndices, m_header.boneIndexSize);
				Read(stream, blending.weight);
			}
			break;
			case PMXFile::VERTEX_BDEF4:
			{
				auto& blending = vertex.blending.bdef4;
				Read(stream, blending.boneIndices, m_header.boneIndexSize);
				Read(stream, blending.weights);
			}
			break;
			case PMXFile::VERTEX_SDEF:
			{
				auto& blending = vertex.blending.sdef;
				Read(stream, blending.boneIndices, m_header.boneIndexSize);
				Read(stream, blending.weight);
				Read(stream, blending.c);
				Read(stream, blending.r0);
				Read(stream, blending.r1);
			}
			break;
			default:
				throw PMXParseError("Unknown blending type");
			}

			Read(stream, vertex.edgeWeight);
		}
	}

	void PMXFile::ParseFace(std::ifstream& stream)
	{
		uint32_t elementCount;
		Read(stream, elementCount);
		elementCount /= 3;
		m_faces.resize(elementCount);
		for (uint32_t i = 0; i < elementCount; ++i) {
			Face& face = m_faces[i];
			Read(stream, face.vertexIndices, m_header.vertexIndexSize);
		}
	}

	void PMXFile::ParseTexture(std::ifstream& stream)
	{
		uint32_t textureCount;
		Read(stream, textureCount);
		m_textures.resize(textureCount);
		for (uint32_t i = 0; i < textureCount; ++i) {
			Texture& texture = m_textures[i];
			ParseText(stream, texture.name);
		}
	}

	void PMXFile::ParseMaterial(std::ifstream& stream)
	{
		uint32_t materialCount;
		Read(stream, materialCount);
		m_materials.resize(materialCount);
		for (uint32_t i = 0; i < materialCount; ++i) {
			Material& material = m_materials[i];
			ParseText(stream, material.nameJP);
			ParseText(stream, material.nameEN);
			Read(stream, material.diffuseColor);
			Read(stream, material.specularColor);
			Read(stream, material.specularExponent);
			Read(stream, material.ambientColor);
			Read(stream, material.drawFlag);
			Read(stream, material.edgeColor);
			Read(stream, material.edgeWeight);
			Read(stream, material.textureIndex, m_header.textureIndexSize);
			Read(stream, material.sphereIndex, m_header.textureIndexSize);
			Read(stream, material.sphereMode);
			Read(stream, material.toonFlag);

			if (material.toonFlag)
				Read(stream, material.toonIndex, 1);
			else
				Read(stream, material.toonIndex, m_header.textureIndexSize);

			ParseText(stream, material.comment);
			Read(stream, material.elementCount);
		}
	}

	void PMXFile::ParseBone(std::ifstream& stream)
	{
		uint32_t boneCount;
		Read(stream, boneCount);
		m_bones.resize(boneCount);
		for (uint32_t i = 0; i < boneCount; ++i) {
			Bone& bone = m_bones[i];
			ParseText(stream, bone.nameJP);
			ParseText(stream, bone.nameEN);
			Read(stream, bone.position);
			Read(stream, bone.parentIndex, m_header.boneIndexSize);
			Read(stream, bone.layer);
			Read(stream, bone.flags);

			if (bone.flags & PMXFile::BONE_CONNECTED_BIT) {
				Read(stream, bone.connetcionEnd.boneIndex, m_header.boneIndexSize);
			}
			else {
				Read(stream, bone.connetcionEnd.position);
			}

			if (bone.flags & PMXFile::BONE_ASSIGN_ROTATION_BIT ||
				bone.flags & PMXFile::BONE_ASSIGN_MOVE_BIT) {
				Read(stream, bone.assignment.targetIndex, m_header.boneIndexSize);
				Read(stream, bone.assignment.ratio);
			}

			if (bone.flags & PMXFile::BONE_FIXED_AXIS_BIT) {
				Read(stream, bone.fixedAxis);
			}

			if (bone.flags & PMXFile::BONE_LOCAL_AXIS_BIT) {
				Read(stream, bone.localAxisX);
				Read(stream, bone.localAxisZ);
			}

			if (bone.flags & PMXFile::BONE_EXTERNAL_PARENT_BIT) {
				Read(stream, bone.externalParentKey);
			}

			if (bone.flags & PMXFile::BONE_IK_BIT) {
				Read(stream, bone.ik.targetIndex, m_header.boneIndexSize);
				Read(stream, bone.ik.iteration);
				Read(stream, bone.ik.unitAngle);

				uint32_t linkSize;
				Read(stream, linkSize);
				bone.ik.link.resize(linkSize);
				for (uint32_t j = 0; j < linkSize; ++j) {
					Bone::IKNode& node = bone.ik.link[j];
					Read(stream, node.boneIndex, m_header.boneIndexSize);
					Read(stream, node.doLimit);
					if (node.doLimit) {
						Read(stream, node.limit[0]);
						Read(stream, node.limit[1]);
					}
				}
			}
		}
	}

	void PMXFile::ParseMorph(std::ifstream& stream)
	{
		uint32_t morphCount;
		Read(stream, morphCount);
		m_morphs.resize(morphCount);
		for (uint32_t i = 0; i < morphCount; ++i) {
			Morph& morph = m_morphs[i];
			ParseText(stream, morph.nameJP);
			ParseText(stream, morph.nameEN);
			Read(stream, morph.panel);
			Read(stream, morph.type);

			uint32_t offsetCount;
			Read(stream, offsetCount);
			morph.offsets.resize(offsetCount);
			for (uint32_t j = 0; j < offsetCount; ++j) {
				switch (morph.type) {
				case PMXFile::MORPH_VERTEX:
				{
					Morph::Offset::Vertex& offset = morph.offsets[j].vertex;
					Read(stream, offset.vertexIndex, m_header.vertexIndexSize);
					Read(stream, offset.value);
				}
				break;
				case PMXFile::MORPH_UV:
				{
					Morph::Offset::UV& offset = morph.offsets[j].uv;
					Read(stream, offset.vertexIndex, m_header.vertexIndexSize);
					Read(stream, offset.value);
				}
				break;
				case PMXFile::MORPH_BONE:
				{
					Morph::Offset::Bone& offset = morph.offsets[j].bone;
					Read(stream, offset.boneIndex, m_header.boneIndexSize);
					Read(stream, offset.move);
					Read(stream, offset.rotation);
				}
				break;
				case PMXFile::MORPH_MATERIAL:
				{
					Morph::Offset::Material& offset = morph.offsets[j].material;
					Read(stream, offset.materialIndex, m_header.materialIndexSize);
					Read(stream, offset.mode);
					Read(stream, offset.diffuseColor);
					Read(stream, offset.specularColor);
					Read(stream, offset.specularExponent);
					Read(stream, offset.ambientColor);
					Read(stream, offset.edgeColor);
					Read(stream, offset.edgeWeight);
					Read(stream, offset.textureTint);
					Read(stream, offset.environtMapTint);
					Read(stream, offset.toonTint);
				}
				break;
				case PMXFile::MORPH_GROUP:
				{
					Morph::Offset::Group& offset = morph.offsets[j].group;
					Read(stream, offset.morphIndex, m_header.morphIndexSize);
					Read(stream, offset.ratio);
				}
				break;
				default:
					throw PMXParseError("Unknown morph type");
				}
			}
		}
	}

	void PMXFile::ParseFrame(std::ifstream& stream)
	{
		uint32_t frameCount;
		Read(stream, frameCount);
		m_clusters.resize(frameCount);
		for (uint32_t i = 0; i < frameCount; ++i) {
			Cluster& cluster = m_clusters[i];
			ParseText(stream, cluster.nameJP);
			ParseText(stream, cluster.nameEN);
			Read(stream, cluster.isSpecial);

			uint32_t elementCount;
			Read(stream, elementCount);
			cluster.elements.resize(elementCount);
			for (uint32_t j = 0; j < elementCount; ++j) {
				Cluster::Element& element = cluster.elements[j];
				Read(stream, element.type);
				if (element.type == PMXFile::CLUSTER_BONE)
					Read(stream, element.index, m_header.boneIndexSize);
				else
					Read(stream, element.index, m_header.morphIndexSize);
			}
		}
	}

	void PMXFile::ParseRigidbody(std::ifstream& stream)
	{
		uint32_t rigidbodyCount;
		Read(stream, rigidbodyCount);
		m_rigidbodies.resize(rigidbodyCount);
		for (uint32_t i = 0; i < rigidbodyCount; ++i) {
			Rigidbody& rigidbody = m_rigidbodies[i];
			ParseText(stream, rigidbody.nameJP);
			ParseText(stream, rigidbody.nameEN);
			Read(stream, rigidbody.boneIndex, m_header.boneIndexSize);
			Read(stream, rigidbody.group);
			Read(stream, rigidbody.noCollisionGroup);
			Read(stream, rigidbody.shape);
			Read(stream, rigidbody.size);
			Read(stream, rigidbody.position);
			Read(stream, rigidbody.rotation);
			Read(stream, rigidbody.mass);
			Read(stream, rigidbody.linearDamping);
			Read(stream, rigidbody.angularDamping);
			Read(stream, rigidbody.restitution);
			Read(stream, rigidbody.friction);
			Read(stream, rigidbody.physicsType);
		}
	}

	void PMXFile::ParseJoint(std::ifstream& stream)
	{
		uint32_t jointCount;
		Read(stream, jointCount);
		m_joints.resize(jointCount);
		for (uint32_t i = 0; i < jointCount; ++i) {
			Joint& joint = m_joints[i];
			ParseText(stream, joint.nameJP);
			ParseText(stream, joint.nameEN);
			Read(stream, joint.type);
			if (joint.type == 0) {
				Read(stream, joint.rigidbodyIndices, m_header.rigidbodyIndexSize);
				Read(stream, joint.position);
				Read(stream, joint.rotation);
				Read(stream, joint.linearLimit[0]);
				Read(stream, joint.linearLimit[1]);
				Read(stream, joint.angularLimit[0]);
				Read(stream, joint.angularLimit[1]);
				Read(stream, joint.linearStiffness);
				Read(stream, joint.angularStiffness);
			}
			else {
				throw PMXParseError("Unknown joint type");
			}
		}
	}

	PMXFile::PMXFile(const std::filesystem::path& path)
	{
		m_path = path;
		std::ifstream stream;
		stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		stream.open(path, std::ifstream::binary);
		ParseHeader(stream);
		ParseInfo(stream);
		ParseVertex(stream);
		ParseFace(stream);
		ParseTexture(stream);
		ParseMaterial(stream);
		ParseBone(stream);
		ParseMorph(stream);
		ParseFrame(stream);
		ParseRigidbody(stream);
		ParseJoint(stream);
	}
}
