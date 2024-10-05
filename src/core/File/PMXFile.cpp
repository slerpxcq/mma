#include "CorePch.hpp"
#include "PMXFile.hpp"

namespace mm
{

template <typename Dst>
static void Read(InFileStream& stream, Dst& dst, i32 srcSize = sizeof(Dst))
{
	char tmp[4]{};
	stream.read(tmp, srcSize);

	switch (srcSize) {
	case 1:
		dst = static_cast<Dst>(*reinterpret_cast<i8*>(&tmp));
		break;
	case 2:
		dst = static_cast<Dst>(*reinterpret_cast<i16*>(&tmp));
		break;
	case 4:
		/* Both f32 and i32 */
		dst = *reinterpret_cast<Dst*>(&tmp);
		break;
	default:
		MM_UNREACHABLE();
	}
}

template <typename Dst, i32 N>
static void Read(InFileStream& stream, Dst(&buffer)[N], i32 size = sizeof(Dst))
{
	for (i32 i = 0; i < N; ++i) {
		Read(stream, buffer[i], size);
	}
}

Ref<PMXFile> PMXParser::Parse(const Path& path) 
{
	InFileStream stream{};
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

	return m_pmx;
}

String PMXParser::ParseText(InFileStream& stream)
{
	u32 length{};
	Read(stream, length);

	String str{};
	str.resize(length);

	stream.read(&str[0], length);

	switch (static_cast<PMXFile::TextEncoding>(m_pmx->header.textEncoding)) { 
	case PMXFile::TextEncoding::UTF16LE:
		return Locale::UTF16LEToUTF8(str.c_str(), str.length());
	default: 
		MM_UNINPLEMENTED("UTF8");
	}
}

void PMXParser::ParseHeader(InFileStream& stream)
{
	Read(stream, m_pmx->header.magic);
	if (memcmp(m_pmx->header.magic, "PMX ", 4)) {
		throw PMXParseError("Invalid file");
	}

	Read(stream, m_pmx->header.version);
	Read(stream, m_pmx->header.dataCount);
	Read(stream, m_pmx->header.textEncoding);
	Read(stream, m_pmx->header.extraUVCount);

	if (m_pmx->header.extraUVCount > 0) {
		MM_UNINPLEMENTED("Extra UV");
	}

	Read(stream, m_pmx->header.vertexIndexSize);
	Read(stream, m_pmx->header.textureIndexSize);
	Read(stream, m_pmx->header.materialIndexSize);
	Read(stream, m_pmx->header.boneIndexSize);
	Read(stream, m_pmx->header.morphIndexSize);
	Read(stream, m_pmx->header.rigidbodyIndexSize);
}

void PMXParser::ParseInfo(InFileStream& stream)
{
	m_pmx->info.nameJP = ParseText(stream);
	m_pmx->info.nameEN = ParseText(stream);
	m_pmx->info.commentJP = ParseText(stream);
	m_pmx->info.commentEN = ParseText(stream);
}

void PMXParser::ParseVertex(InFileStream& stream)
{
	i32 vertexCount{};
	Read(stream, vertexCount);
	m_pmx->vertices.resize(vertexCount);
	for (i32 i = 0; i < vertexCount; ++i) {
		PMXFile::Vertex& vertex = m_pmx->vertices[i];
		Read(stream, vertex.position);
		Read(stream, vertex.normal);
		Read(stream, vertex.uv);

		Read(stream, vertex.blendingType);
		switch (static_cast<PMXFile::BlendingType>(vertex.blendingType)) {
		case PMXFile::BlendingType::BDEF1:
		{
			auto& blending = vertex.blending.bdef1;
			Read(stream, blending.boneIndex, m_pmx->header.boneIndexSize);
		}
		break;
		case PMXFile::BlendingType::BDEF2:
		{
			auto& blending = vertex.blending.bdef2;
			Read(stream, blending.boneIndices, m_pmx->header.boneIndexSize);
			Read(stream, blending.weight);
		}
		break;
		case PMXFile::BlendingType::BDEF4:
		{
			auto& blending = vertex.blending.bdef4;
			Read(stream, blending.boneIndices, m_pmx->header.boneIndexSize);
			Read(stream, blending.weights);
		}
		break;
		case PMXFile::BlendingType::SDEF:
		{
			auto& blending = vertex.blending.sdef;
			Read(stream, blending.boneIndices, m_pmx->header.boneIndexSize);
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

void PMXParser::ParseFace(InFileStream& stream)
{
	i32 elementCount{};
	Read(stream, elementCount);
	elementCount /= 3;
	m_pmx->faces.resize(elementCount);

	for (i32 i = 0; i < elementCount; ++i) {
		PMXFile::Face& face = m_pmx->faces[i];
		Read(stream, face.vertexIndices, m_pmx->header.vertexIndexSize);
	}
}

void PMXParser::ParseTexture(InFileStream& stream)
{
	i32 textureCount{};
	Read(stream, textureCount);
	m_pmx->textures.resize(textureCount);

	for (i32 i = 0; i < textureCount; ++i) {
		PMXFile::Texture& texture = m_pmx->textures[i];
		texture.name = ParseText(stream);
	}
}

void PMXParser::ParseMaterial(InFileStream& stream)
{
	i32 materialCount{};
	Read(stream, materialCount);
	m_pmx->materials.resize(materialCount);
	for (i32 i = 0; i < materialCount; ++i) {
		PMXFile::Material& material = m_pmx->materials[i];
		material.nameJP = ParseText(stream);
		material.nameEN = ParseText(stream);
		Read(stream, material.diffuseColor);
		Read(stream, material.specularColor);
		Read(stream, material.specularExponent);
		Read(stream, material.ambientColor);
		Read(stream, material.drawFlag);
		Read(stream, material.edgeColor);
		Read(stream, material.edgeWeight);
		Read(stream, material.textureIndex, m_pmx->header.textureIndexSize);
		Read(stream, material.sphereIndex, m_pmx->header.textureIndexSize);
		Read(stream, material.sphereMode);
		Read(stream, material.toonFlag);

		if (material.toonFlag) {
			Read(stream, material.toonIndex, 1);
		}
		else {
			Read(stream, material.toonIndex, m_pmx->header.textureIndexSize);
		}

		material.comment = ParseText(stream);
		Read(stream, material.elementCount);
	}
}

void PMXParser::ParseBone(InFileStream& stream)
{
	i32 boneCount{};
	Read(stream, boneCount);
	m_pmx->bones.resize(boneCount);
	for (i32 i = 0; i < boneCount; ++i) {
		PMXFile::Bone& bone = m_pmx->bones[i];
		bone.nameJP = ParseText(stream);
		bone.nameEN = ParseText(stream);
		Read(stream, bone.position);
		Read(stream, bone.parentIndex, m_pmx->header.boneIndexSize);
		Read(stream, bone.transformationLayer);
		Read(stream, bone.flags);

		if (bone.flags & PMXFile::BoneFlag::CONNECTED_BIT) {
			Read(stream, bone.connetcionEnd.boneIndex, m_pmx->header.boneIndexSize);
		}
		else {
			Read(stream, bone.connetcionEnd.position);
		}

		if (bone.flags & PMXFile::BoneFlag::ASSIGN_ROTATION_BIT ||
			bone.flags & PMXFile::BoneFlag::ASSIGN_MOVE_BIT) {
			Read(stream, bone.assignment.targetIndex, m_pmx->header.boneIndexSize);
			Read(stream, bone.assignment.ratio);
		}

		if (bone.flags & PMXFile::BoneFlag::FIXED_AXIS_BIT) {
			Read(stream, bone.fixedAxis);
		}

		if (bone.flags & PMXFile::BoneFlag::LOCAL_AXIS_BIT) {
			Read(stream, bone.localAxisX);
			Read(stream, bone.localAxisZ);
		}

		if (bone.flags & PMXFile::BoneFlag::EXTERNAL_PARENT_BIT) {
			Read(stream, bone.externalParentKey);
		}

		if (bone.flags & PMXFile::BoneFlag::IK_BIT) {
			Read(stream, bone.ik.targetIndex, m_pmx->header.boneIndexSize);
			Read(stream, bone.ik.iteration);
			Read(stream, bone.ik.unitAngle);

			i32 linkSize{};
			Read(stream, linkSize);
			bone.ik.link.resize(linkSize);
			for (i32 j = 0; j < linkSize; ++j) {
				PMXFile::Bone::IKNode& node = bone.ik.link[j];
				Read(stream, node.boneIndex, m_pmx->header.boneIndexSize);
				Read(stream, node.doLimit);
				if (node.doLimit) {
					Read(stream, node.limit[0]);
					Read(stream, node.limit[1]);
				}
			}
		}
	}
}

void PMXParser::ParseMorph(InFileStream& stream)
{
	i32 morphCount{};
	Read(stream, morphCount);
	m_pmx->morphs.resize(morphCount);
	for (i32 i = 0; i < morphCount; ++i) {
		PMXFile::Morph& morph = m_pmx->morphs[i];
		morph.nameJP = ParseText(stream);
		morph.nameEN = ParseText(stream);
		Read(stream, morph.panel);
		Read(stream, morph.type);

		i32 offsetCount{};
		Read(stream, offsetCount);
		morph.offsets.resize(offsetCount);
		for (uint32_t j = 0; j < offsetCount; ++j) {
			switch (static_cast<PMXFile::MorphType>(morph.type)) {
			case PMXFile::MorphType::VERTEX:
			{
				PMXFile::Morph::Offset::Vertex& offset = morph.offsets[j].vertex;
				Read(stream, offset.vertexIndex, m_pmx->header.vertexIndexSize);
				Read(stream, offset.value);
			}
			break;
			case PMXFile::MorphType::UV:
			{
				PMXFile::Morph::Offset::UV& offset = morph.offsets[j].uv;
				Read(stream, offset.vertexIndex, m_pmx->header.vertexIndexSize);
				Read(stream, offset.value);
			}
			break;
			case PMXFile::MorphType::BONE:
			{
				PMXFile::Morph::Offset::Bone& offset = morph.offsets[j].bone;
				Read(stream, offset.boneIndex, m_pmx->header.boneIndexSize);
				Read(stream, offset.move);
				Read(stream, offset.rotation);
			}
			break;
			case PMXFile::MorphType::MATERIAL:
			{
				PMXFile::Morph::Offset::Material& offset = morph.offsets[j].material;
				Read(stream, offset.materialIndex, m_pmx->header.materialIndexSize);
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
			case PMXFile::MorphType::GROUP:
			{
				PMXFile::Morph::Offset::Group& offset = morph.offsets[j].group;
				Read(stream, offset.morphIndex, m_pmx->header.morphIndexSize);
				Read(stream, offset.ratio);
			}
			break;
			default:
				throw PMXParseError("Unknown morph type");
			}
		}
	}
}

void PMXParser::ParseFrame(InFileStream& stream)
{
	i32 frameCount{};
	Read(stream, frameCount);
	m_pmx->clusters.resize(frameCount);
	for (i32 i = 0; i < frameCount; ++i) {
		PMXFile::Cluster& cluster = m_pmx->clusters[i];
		cluster.nameJP = ParseText(stream);
		cluster.nameEN = ParseText(stream);
		Read(stream, cluster.isSpecial);

		i32 elementCount{};
		Read(stream, elementCount);
		cluster.elements.resize(elementCount);
		for (i32 j = 0; j < elementCount; ++j) {
			PMXFile::Cluster::Element& element = cluster.elements[j];
			Read(stream, element.type);
			switch (static_cast<PMXFile::ClusterType>(element.type)) {
			case PMXFile::ClusterType::BONE:
				Read(stream, element.index, m_pmx->header.boneIndexSize);
				break;
			case PMXFile::ClusterType::MORPH:
				Read(stream, element.index, m_pmx->header.morphIndexSize);
				break;
			default:
				;
				throw PMXParseError("Unknown cluster type");
			}
		}
	}
}

void PMXParser::ParseRigidbody(InFileStream& stream)
{
	i32 rigidbodyCount{};
	Read(stream, rigidbodyCount);
	m_pmx->rigidbodies.resize(rigidbodyCount);

	for (i32 i = 0; i < rigidbodyCount; ++i) {
		PMXFile::Rigidbody& rigidbody = m_pmx->rigidbodies[i];
		rigidbody.nameJP = ParseText(stream);
		rigidbody.nameEN = ParseText(stream);
		Read(stream, rigidbody.boneIndex, m_pmx->header.boneIndexSize);
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

void PMXParser::ParseJoint(InFileStream& stream)
{
	i32 jointCount{};
	Read(stream, jointCount);
	m_pmx->joints.resize(jointCount);

	for (i32 i = 0; i < jointCount; ++i) {
		PMXFile::Joint& joint = m_pmx->joints[i];
		joint.nameJP = ParseText(stream);
		joint.nameEN = ParseText(stream);
		Read(stream, joint.type);
		switch (static_cast<PMXFile::JointType>(joint.type)) {
		case PMXFile::JointType::GENERIC_6DOF_SPRING:
			Read(stream, joint.rigidbodyIndices, m_pmx->header.rigidbodyIndexSize);
			Read(stream, joint.position);
			Read(stream, joint.rotation);
			Read(stream, joint.linearLimit[0]);
			Read(stream, joint.linearLimit[1]);
			Read(stream, joint.angularLimit[0]);
			Read(stream, joint.angularLimit[1]);
			Read(stream, joint.linearStiffness);
			Read(stream, joint.angularStiffness);
			break;
		default:
			MM_UNINPLEMENTED("joint type");
		}
	}
}

}
