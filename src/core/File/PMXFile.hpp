#pragma once

#include "Common/Locale.hpp"

namespace mm
{
struct PMXParseError : public MMException
{
	PMXParseError(const char* what = "") : MMException(what) {}
};

struct PMXFile
{
	enum class TextEncoding : u8 {
		UTF16LE = 0,
		UTF8
	};

	enum class BlendingType : u8 {
		BDEF1 = 0,
		BDEF2,
		BDEF4,
		SDEF
	};

	enum BoneFlag : u16 {
		CONNECTED_BIT = (1U << 0),
		ROTATABLE_BIT = (1U << 1),
		MOVEABLE_BIT = (1U << 2),
		VISIBLE_BIT = (1U << 3),
		OPERABLE_BIT = (1U << 4),
		IK_BIT = (1U << 5),
		ASSIGN_LOCAL_BIT = (1U << 7),
		ASSIGN_ROTATION_BIT = (1U << 8),
		ASSIGN_MOVE_BIT = (1U << 9),
		FIXED_AXIS_BIT = (1U << 10),
		LOCAL_AXIS_BIT = (1U << 11),
		AFTER_PHYSICS_BIT = (1U << 12),
		EXTERNAL_PARENT_BIT = (1U << 13),
	};

	enum class MaterialFlag : u8 {
		NO_CULL_BIT = 1 << 0,
		GROUND_SHADOW_BIT = 1 << 1,
		SELF_SHADOW_MAP_BIT = 1 << 2,
		SELF_SHADOW_BIT = 1 << 3,
		EDGE_BIT = 1 << 4
	};

	enum class SphType : u8 {
		DISABLE = 0,
		MULTIPLY,
		ADD,
		SUBTEXTURE
	};

	enum class ToonFlag : u8 {
		TOON_SHARED_BIT = 1,
	};

	enum class MorphType : u8 {
		GROUP = 0,
		VERTEX,
		BONE,
		UV,
		EXTRA_UV_1,
		EXTRA_UV_2,
		EXTRA_UV_3,
		EXTRA_UV_4,
		MATERIAL,
	};

	enum class MorphMethod : u8 {
		MULTIPLY = 0,
		ADD
	};

	enum class Panel : u8 {
		RESERVED = 0,
		EYEBROW,
		EYE,
		MOUTH,
		OTHER,
	};

	enum class ClusterType : u8 {
		BONE = 0,
		MORPH
	};

	enum class RigidbodyShape : u8 {
		SPHERE = 0,
		BOX,
		CAPSULE
	};

	enum class RigidbodyType : u8 {
		KINEMATIC = 0,
		DYNAMIC,
		PIVOTED
	};

	enum class JointType : u8 {
		GENERIC_6DOF_SPRING = 0,
		GENERIC_6DOF,
		P2P,
		CONE_TWIST,
		SLIDER,
		HINGE
	};

	struct Header {
		char magic[4];
		f32  version;
		u8 dataCount;
		u8 textEncoding;
		u8 extraUVCount;
		u8 vertexIndexSize;
		u8 textureIndexSize;
		u8 materialIndexSize;
		u8 boneIndexSize;
		u8 morphIndexSize;
		u8 rigidbodyIndexSize;
	};

	struct Info {
		String nameJP;
		String nameEN;
		String commentJP;
		String commentEN;
	};

	struct Vertex {
		union Blending {
			struct BDEF1 {
				i32 boneIndex;
			} bdef1;

			struct BDEF2 {
				i32 boneIndices[2];
				f32 weight;
			} bdef2;

			struct BDEF4 {
				i32 boneIndices[4];
				f32   weights[4];
			} bdef4;

			struct SDEF {
				i32 boneIndices[2];
				f32   weight;
				f32   c[3];
				f32   r0[3];
				f32   r1[3];
			} sdef;
		};

		f32    position[3];
		f32    normal[3];
		f32    uv[2];
		u8  blendingType;
		Blending blending;
		f32    edgeWeight;
	};

	struct Face {
		i32 vertexIndices[3];
	};

	struct Texture {
		String name;
	};

	struct Material {
		String nameJP;
		String nameEN;
		f32       diffuseColor[4];
		f32       specularColor[3];
		f32       specularExponent;
		f32       ambientColor[3];
		u8     drawFlag;
		f32       edgeColor[4];
		f32       edgeWeight;
		i32     textureIndex;
		i32     sphereIndex;
		u8     sphereMode;
		u8     toonFlag;
		i32     toonIndex;
		String comment;
		i32     elementCount;
	};

	struct Bone {
		struct IKNode {
			i32 boneIndex;
			u8 doLimit;
			f32   limit[2][3];
		};

		struct Assignment {
			i32 targetIndex;
			f32   ratio;
		};

		struct IK {
			i32             targetIndex;
			i32             iteration;
			f32               unitAngle;
			DynArray<IKNode> link;
		};

		union ConnectionEnd {
			i32 boneIndex;
			f32 position[3];
		};

		String   nameJP;
		String   nameEN;
		f32         position[3];
		i32       parentIndex;
		i32       transformationLayer;
		u16      flags;
		ConnectionEnd connetcionEnd;
		Assignment    assignment;
		f32         fixedAxis[3];
		f32         localAxisX[3];
		f32         localAxisZ[3];
		i32       externalParentKey;
		IK            ik;
	};

	struct Morph {
		union Offset {
			struct Group {
				i32 morphIndex;
				f32   ratio;
			} group;

			struct Vertex {
				i32 vertexIndex;
				f32   value[3];
			} vertex;

			struct Bone {
				i32 boneIndex;
				f32   move[3];
				f32   rotation[4];
			} bone;

			struct UV {
				i32 vertexIndex;
				f32   value[4];
			} uv;

			struct Material {
				i32 materialIndex;
				u8 mode;
				f32   diffuseColor[4];
				f32   specularColor[3];
				f32   specularExponent;
				f32   ambientColor[3];
				f32   edgeColor[4];
				f32   edgeWeight;
				f32   textureTint[4];
				f32   environtMapTint[4];
				f32   toonTint[4];
			} material;
		};

		String         nameJP;
		String         nameEN;
		u8             panel;
		u8             type;
		DynArray<Offset> offsets;
	};

	struct Cluster {
		struct Element {
			u8 type;
			i32 index;
		};

		String nameJP;
		String nameEN;
		u8 isSpecial;
		DynArray<Element> elements;
	};

	struct Rigidbody {
		String nameJP;
		String nameEN;
		i32     boneIndex;
		u8     group;
		u16    noCollisionGroup;
		u8     shape;
		f32       size[3];
		f32       position[3];
		f32       rotation[3];
		f32       mass;
		f32       linearDamping;
		f32       angularDamping;
		f32       restitution;
		f32       friction;
		u8     physicsType;
	};

	struct Joint {
		String nameJP;
		String nameEN;
		u8     type;
		i32     rigidbodyIndices[2];
		f32       position[3];
		f32       rotation[3];
		f32       linearLimit[2][3];
		f32       angularLimit[2][3];
		f32       linearStiffness[3];
		f32       angularStiffness[3];
	};

public:
	static PMXFile Load(const Path& path);

public:
	Header                 header{};
	Info                   info{};
	DynArray<Vertex>    vertices{};
	DynArray<Face>      faces{};
	DynArray<Texture>   textures{};
	DynArray<Material>  materials{};
	DynArray<Bone>      bones{};
	DynArray<Morph>     morphs{};
	DynArray<Cluster>   sequence{};
	DynArray<Rigidbody> rigidbodies{};
	DynArray<Joint>     joints{};
};

class PMXParser
{
public:
	PMXFile Parse(const Path& path);
	/* void Serialize(const PMXFile& pmx, const Path& path);*/

private:
	String ParseText(InFileStream& stream);
	void ParseHeader(InFileStream& stream);
	void ParseInfo(InFileStream& stream);
	void ParseVertex(InFileStream& stream);
	void ParseFace(InFileStream& stream);
	void ParseTexture(InFileStream& stream);
	void ParseMaterial(InFileStream& stream);
	void ParseBone(InFileStream& stream);
	void ParseMorph(InFileStream& stream);
	void ParseFrame(InFileStream& stream);
	void ParseRigidbody(InFileStream& stream);
	void ParseJoint(InFileStream& stream);

private:
	PMXFile m_pmx{};
};


}
