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
		MM_CORE_UNREACHABLE();
	}
}

template <typename Dst, i32 N>
static void Read(InFileStream& stream, Dst(&buffer)[N], i32 size = sizeof(Dst))
{
	for (i32 i = 0; i < N; ++i) {
		Read(stream, buffer[i], size);
	}
}

Ref<PMXFile> PMXFile::Load(const Path& path) 
{
	auto pmx = MakeRef<PMXFile>(path);

	InFileStream stream{};
	stream.exceptions(stream.failbit | stream.badbit);
	stream.open(path, stream.binary);

	ParseHeader(*pmx, stream);
	ParseInfo(*pmx, stream);
	ParseVertex(*pmx, stream);
	ParseFace(*pmx, stream);
	ParseTexture(*pmx, stream);
	ParseMaterial(*pmx, stream);
	ParseBone(*pmx, stream);
	ParseMorph(*pmx, stream);
	ParseFrame(*pmx, stream);
	ParseRigidbody(*pmx, stream);
	ParseJoint(*pmx, stream);

	return pmx;
}

String PMXFile::ParseText(const PMXFile& pmx, InFileStream& stream)
{
	u32 length{};
	Read(stream, length);

	String str{};
	str.resize(length);

	stream.read(&str[0], length);

	switch (static_cast<PMXFile::TextEncoding>(pmx.m_header.textEncoding)) { 
	case PMXFile::TextEncoding::UTF16LE:
		return Locale::UTF16LEToUTF8(str.c_str(), str.length());
	default: 
		MM_CORE_UNINPLEMENTED("UTF8");
	}
}

void PMXFile::ParseHeader(PMXFile& pmx, InFileStream& stream)
{
	Read(stream, pmx.m_header.magic);
	if (memcmp(pmx.m_header.magic, "PMX ", 4)) {
		throw PMXParseError("Invalid file");
	}

	Read(stream, pmx.m_header.version);
	Read(stream, pmx.m_header.dataCount);
	Read(stream, pmx.m_header.textEncoding);
	Read(stream, pmx.m_header.extraUVCount);

	if (pmx.m_header.extraUVCount > 0) {
		MM_CORE_UNINPLEMENTED("Extra UV");
	}

	Read(stream, pmx.m_header.vertexIndexSize);
	Read(stream, pmx.m_header.textureIndexSize);
	Read(stream, pmx.m_header.materialIndexSize);
	Read(stream, pmx.m_header.boneIndexSize);
	Read(stream, pmx.m_header.morphIndexSize);
	Read(stream, pmx.m_header.rigidbodyIndexSize);
}

void PMXFile::ParseInfo(PMXFile& pmx, InFileStream& stream)
{
	pmx.m_info.nameJP = ParseText(pmx, stream);
	pmx.m_info.nameEN = ParseText(pmx, stream);
	pmx.m_info.commentJP = ParseText(pmx, stream);
	pmx.m_info.commentEN = ParseText(pmx, stream);
}

void PMXFile::ParseVertex(PMXFile& pmx, InFileStream& stream)
{
	i32 vertexCount{};
	Read(stream, vertexCount);
	pmx.m_vertices.resize(vertexCount);
	for (i32 i = 0; i < vertexCount; ++i) {
		PMXFile::Vertex& vertex = pmx.m_vertices[i];
		Read(stream, vertex.position);
		Read(stream, vertex.normal);
		Read(stream, vertex.uv);

		Read(stream, vertex.blendingType);
		switch (static_cast<PMXFile::BlendingType>(vertex.blendingType)) {
		case PMXFile::BlendingType::BDEF1:
		{
			auto& blending = vertex.blending.bdef1;
			Read(stream, blending.boneIndex, pmx.m_header.boneIndexSize);
		}
		break;
		case PMXFile::BlendingType::BDEF2:
		{
			auto& blending = vertex.blending.bdef2;
			Read(stream, blending.boneIndices, pmx.m_header.boneIndexSize);
			Read(stream, blending.weight);
		}
		break;
		case PMXFile::BlendingType::BDEF4:
		{
			auto& blending = vertex.blending.bdef4;
			Read(stream, blending.boneIndices, pmx.m_header.boneIndexSize);
			Read(stream, blending.weights);
		}
		break;
		case PMXFile::BlendingType::SDEF:
		{
			auto& blending = vertex.blending.sdef;
			Read(stream, blending.boneIndices, pmx.m_header.boneIndexSize);
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

void PMXFile::ParseFace(PMXFile& pmx, InFileStream& stream)
{
	i32 elementCount{};
	Read(stream, elementCount);
	elementCount /= 3;
	pmx.m_faces.resize(elementCount);

	for (i32 i = 0; i < elementCount; ++i) {
		PMXFile::Face& face = pmx.m_faces[i];
		Read(stream, face.vertexIndices, pmx.m_header.vertexIndexSize);
	}
}

void PMXFile::ParseTexture(PMXFile& pmx, InFileStream& stream)
{
	i32 textureCount{};
	Read(stream, textureCount);
	pmx.m_textures.resize(textureCount);

	for (i32 i = 0; i < textureCount; ++i) {
		PMXFile::Texture& texture = pmx.m_textures[i];
		texture.name = ParseText(pmx, stream);
	}
}

void PMXFile::ParseMaterial(PMXFile& pmx, InFileStream& stream)
{
	i32 materialCount{};
	Read(stream, materialCount);
	pmx.m_materials.resize(materialCount);
	for (i32 i = 0; i < materialCount; ++i) {
		PMXFile::Material& material = pmx.m_materials[i];
		material.nameJP = ParseText(pmx, stream);
		material.nameEN = ParseText(pmx, stream);
		Read(stream, material.diffuseColor);
		Read(stream, material.specularColor);
		Read(stream, material.specularExponent);
		Read(stream, material.ambientColor);
		Read(stream, material.drawFlag);
		Read(stream, material.edgeColor);
		Read(stream, material.edgeWeight);
		Read(stream, material.textureIndex, pmx.m_header.textureIndexSize);
		Read(stream, material.sphereIndex, pmx.m_header.textureIndexSize);
		Read(stream, material.sphereMode);
		Read(stream, material.toonFlag);

		if (material.toonFlag) {
			Read(stream, material.toonIndex, 1);
		}
		else {
			Read(stream, material.toonIndex, pmx.m_header.textureIndexSize);
		}

		material.comment = ParseText(pmx, stream);
		Read(stream, material.elementCount);
	}
}

void PMXFile::ParseBone(PMXFile& pmx, InFileStream& stream)
{
	i32 boneCount{};
	Read(stream, boneCount);
	pmx.m_bones.resize(boneCount);
	for (i32 i = 0; i < boneCount; ++i) {
		PMXFile::Bone& bone = pmx.m_bones[i];
		bone.nameJP = ParseText(pmx, stream);
		bone.nameEN = ParseText(pmx, stream);
		Read(stream, bone.position);
		Read(stream, bone.parentIndex, pmx.m_header.boneIndexSize);
		Read(stream, bone.transformationLayer);
		Read(stream, bone.flags);

		if (bone.flags & PMXFile::BoneFlag::CONNECTED_BIT) {
			Read(stream, bone.connetcionEnd.boneIndex, pmx.m_header.boneIndexSize);
		}
		else {
			Read(stream, bone.connetcionEnd.position);
		}

		if (bone.flags & PMXFile::BoneFlag::ASSIGN_ROTATION_BIT ||
			bone.flags & PMXFile::BoneFlag::ASSIGN_MOVE_BIT) {
			Read(stream, bone.assignment.targetIndex, pmx.m_header.boneIndexSize);
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
			Read(stream, bone.ik.targetIndex, pmx.m_header.boneIndexSize);
			Read(stream, bone.ik.iteration);
			Read(stream, bone.ik.unitAngle);

			i32 linkSize{};
			Read(stream, linkSize);
			bone.ik.link.resize(linkSize);
			for (i32 j = 0; j < linkSize; ++j) {
				PMXFile::Bone::IKNode& node = bone.ik.link[j];
				Read(stream, node.boneIndex, pmx.m_header.boneIndexSize);
				Read(stream, node.doLimit);
				if (node.doLimit) {
					Read(stream, node.limit[0]);
					Read(stream, node.limit[1]);
				}
			}
		}
	}
}

void PMXFile::ParseMorph(PMXFile& pmx, InFileStream& stream)
{
	i32 morphCount{};
	Read(stream, morphCount);
	pmx.m_morphs.resize(morphCount);
	for (i32 i = 0; i < morphCount; ++i) {
		PMXFile::Morph& morph = pmx.m_morphs[i];
		morph.nameJP = ParseText(pmx, stream);
		morph.nameEN = ParseText(pmx, stream);
		Read(stream, morph.panel);
		Read(stream, morph.type);

		i32 offsetCount{};
		Read(stream, offsetCount);
		morph.offsets.resize(offsetCount);
		for (i32 j = 0; j < offsetCount; ++j) {
			switch (static_cast<PMXFile::MorphType>(morph.type)) {
			case PMXFile::MorphType::VERTEX:
			{
				PMXFile::Morph::Offset::Vertex& offset = morph.offsets[j].vertex;
				Read(stream, offset.vertexIndex, pmx.m_header.vertexIndexSize);
				Read(stream, offset.value);
			}
			break;
			case PMXFile::MorphType::UV:
			{
				PMXFile::Morph::Offset::UV& offset = morph.offsets[j].uv;
				Read(stream, offset.vertexIndex, pmx.m_header.vertexIndexSize);
				Read(stream, offset.value);
			}
			break;
			case PMXFile::MorphType::BONE:
			{
				PMXFile::Morph::Offset::Bone& offset = morph.offsets[j].bone;
				Read(stream, offset.boneIndex, pmx.m_header.boneIndexSize);
				Read(stream, offset.move);
				Read(stream, offset.rotation);
			}
			break;
			case PMXFile::MorphType::MATERIAL:
			{
				PMXFile::Morph::Offset::Material& offset = morph.offsets[j].material;
				Read(stream, offset.materialIndex, pmx.m_header.materialIndexSize);
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
				Read(stream, offset.morphIndex, pmx.m_header.morphIndexSize);
				Read(stream, offset.ratio);
			}
			break;
			default:
				throw PMXParseError("Unknown morph type");
			}
		}
	}
}

void PMXFile::ParseFrame(PMXFile& pmx, InFileStream& stream)
{
	i32 frameCount{};
	Read(stream, frameCount);
	pmx.m_clusters.resize(frameCount);
	for (i32 i = 0; i < frameCount; ++i) {
		PMXFile::Cluster& cluster = pmx.m_clusters[i];
		cluster.nameJP = ParseText(pmx, stream);
		cluster.nameEN = ParseText(pmx, stream);
		Read(stream, cluster.isSpecial);

		i32 elementCount{};
		Read(stream, elementCount);
		cluster.elements.resize(elementCount);
		for (i32 j = 0; j < elementCount; ++j) {
			PMXFile::Cluster::Element& element = cluster.elements[j];
			Read(stream, element.type);
			switch (static_cast<PMXFile::ClusterType>(element.type)) {
			case PMXFile::ClusterType::BONE:
				Read(stream, element.index, pmx.m_header.boneIndexSize);
				break;
			case PMXFile::ClusterType::MORPH:
				Read(stream, element.index, pmx.m_header.morphIndexSize);
				break;
			default:
				throw PMXParseError("Unknown cluster type");
			}
		}
	}
}

void PMXFile::ParseRigidbody(PMXFile& pmx, InFileStream& stream)
{
	i32 rigidbodyCount{};
	Read(stream, rigidbodyCount);
	pmx.m_rigidbodies.resize(rigidbodyCount);

	for (i32 i = 0; i < rigidbodyCount; ++i) {
		PMXFile::Rigidbody& rigidbody = pmx.m_rigidbodies[i];
		rigidbody.nameJP = ParseText(pmx, stream);
		rigidbody.nameEN = ParseText(pmx, stream);
		Read(stream, rigidbody.boneIndex, pmx.m_header.boneIndexSize);
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

void PMXFile::ParseJoint(PMXFile& pmx, InFileStream& stream)
{
	i32 jointCount{};
	Read(stream, jointCount);
	pmx.m_joints.resize(jointCount);

	for (i32 i = 0; i < jointCount; ++i) {
		PMXFile::Joint& joint = pmx.m_joints[i];
		joint.nameJP = ParseText(pmx, stream);
		joint.nameEN = ParseText(pmx, stream);
		Read(stream, joint.type);
		switch (static_cast<PMXFile::JointType>(joint.type)) {
		case PMXFile::JointType::GENERIC_6DOF_SPRING:
			Read(stream, joint.rigidbodyIndices, pmx.m_header.rigidbodyIndexSize);
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
			MM_CORE_UNINPLEMENTED("joint type");
		}
	}
}

}
