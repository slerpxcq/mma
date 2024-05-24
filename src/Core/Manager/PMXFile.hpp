#pragma once

#include "File.hpp"
#include "Core/Locale/Locale.hpp"

namespace mm
{
	struct PMXParseError : public std::runtime_error
	{
		PMXParseError(const char* what = "") : std::runtime_error(what) {}
	};

	class PMXFile : public File
	{
	public:
		enum TextEncoding {
			ENC_UTF16LE = 0,
			ENC_UTF8
		};

		enum BlendingType {
			VERTEX_BDEF1 = 0,
			VERTEX_BDEF2,
			VERTEX_BDEF4,
			VERTEX_SDEF
		};

		enum Flag {
			BONE_CONNECTED_BIT = (1U << 0),
			BONE_ROTATABLE_BIT = (1U << 1),
			BONE_MOVEABLE_BIT = (1U << 2),
			BONE_VISIBLE_BIT = (1U << 3),
			BONE_OPERABLE_BIT = (1U << 4),
			BONE_IK_BIT = (1U << 5),
			BONE_ASSIGN_LOCAL_BIT = (1U << 7),
			BONE_ASSIGN_ROTATION_BIT = (1U << 8),
			BONE_ASSIGN_MOVE_BIT = (1U << 9),
			BONE_FIXED_AXIS_BIT = (1U << 10),
			BONE_LOCAL_AXIS_BIT = (1U << 11),
			BONE_AFTER_PHYSICS_BIT = (1U << 12),
			BONE_EXTERNAL_PARENT_BIT = (1U << 13),
		};

		enum MaterialFlag {
			MATERIAL_NO_CULL_BIT = 1 << 0,
			MATERIAL_GROUND_SHADOW_BIT = 1 << 1,
			MATERIAL_SELF_SHADOW_MAP_BIT = 1 << 2,
			MATERIAL_SELF_SHADOW_BIT = 1 << 3,
			MATERIAL_EDGE_BIT = 1 << 4
		};

		enum SphType {
			SPH_DISABLE = 0,
			SPH_MULTIPLY,
			SPH_ADD,
			SPH_SUBTEXTURE
		};

		enum ToonFlag {
			TOON_SHARED_BIT = 1, 
		};

		enum MorphType {
			MORPH_GROUP = 0,
			MORPH_VERTEX,
			MORPH_BONE,
			MORPH_UV,
			MORPH_EXTRA_UV_1,
			MORPH_EXTRA_UV_2,
			MORPH_EXTRA_UV_3,
			MORPH_EXTRA_UV_4,
			MORPH_MATERIAL,
		};

		enum MorphMethod {
			MORPH_MULTIPLY = 0,
			MORPH_ADD
		};

		enum Panel {
			PANEL_RESERVED = 0,
			PANEL_EYEBROW,
			PANEL_EYE,
			PANEL_MOUTH,
			PANEL_OTHER,
		};

		enum ClusterType {
			CLUSTER_BONE = 0,
			CLUSTER_MORPH
		};

		enum RigidbodyShape {
			RB_SPHERE = 0,
			RB_BOX,
			RB_CAPSULE
		};

		enum RigidbodyType {
			RB_KINEMATIC = 0,
			RB_DYNAMIC,
			RB_PIVOTED
		};

		enum JointType {
			JOINT_GENERIC_6DOF_SPRING = 0,
			JOINT_GENERIC_6DOF,
			JOINT_P2P,
			JOINT_CONE_TWIST,
			JOINT_SLIDER,
			JOINT_HINGE
		};

		struct Header {
			char magic[4];
			float   version;
			uint8_t dataCount;
			uint8_t textEncoding;
			uint8_t extraUVCount;
			uint8_t vertexIndexSize;
			uint8_t textureIndexSize;
			uint8_t materialIndexSize;
			uint8_t boneIndexSize;
			uint8_t morphIndexSize;
			uint8_t rigidbodyIndexSize;
		};

		struct Info {
			std::string nameJP;
			std::string nameEN;
			std::string commentJP;
			std::string commentEN;
		};

		struct Vertex {
			union Blending {
				struct BDEF1 {
					int32_t boneIndex;
				} bdef1;

				struct BDEF2 {
					int32_t boneIndices[2];
					float weight;
				} bdef2;

				struct BDEF4 {
					int32_t boneIndices[4];
					float   weights[4];
				} bdef4;

				struct SDEF {
					int32_t boneIndices[2];
					float   weight;
					float   c[3];
					float   r0[3];
					float   r1[3];
				} sdef;
			};

			float    position[3];
			float    normal[3];
			float    uv[2];
			uint8_t  blendingType;
			// std::vector<float[2]> extraUV;
			Blending blending;
			float    edgeWeight;
		};

		struct Face {
			int32_t vertexIndices[3];
		};

		struct Texture {
			std::string name;
		};

		struct Material {
			std::string nameJP;
			std::string nameEN;
			float       diffuseColor[4];
			float       specularColor[3];
			float       specularExponent;
			float       ambientColor[3];
			uint8_t     drawFlag;
			float       edgeColor[4];
			float       edgeWeight;
			int32_t     textureIndex;
			int32_t     sphereIndex;
			uint8_t     sphereMode;
			uint8_t     toonFlag;
			int32_t     toonIndex;
			std::string comment;
			int32_t     elementCount;
		};

		struct Bone {
			struct IKNode {
				int32_t boneIndex;
				uint8_t doLimit;
				float   limit[2][3];
			};

			struct Assignment {
				int32_t targetIndex;
				float   ratio;
			};

			struct IK {
				int32_t             targetIndex;
				int32_t             iteration;
				float               unitAngle;
				std::vector<IKNode> link;
			};

			union ConnectionEnd {
				int32_t boneIndex;
				float position[3];
			};

			std::string   nameJP;
			std::string   nameEN;
			float         position[3];
			int32_t       parentIndex;
			int32_t       layer;
			uint16_t      flags;
			ConnectionEnd connetcionEnd;
			Assignment    assignment;
			float         fixedAxis[3];
			float         localAxisX[3];
			float         localAxisZ[3];
			int32_t       externalParentKey;
			IK            ik;
		};

		struct Morph {
			union Offset {
				struct Group {
					int32_t morphIndex;
					float   ratio;
				} group;

				struct Vertex {
					int32_t vertexIndex;
					float   value[3];
				} vertex;

				struct Bone {
					int32_t boneIndex;
					float   move[3];
					float   rotation[4];
				} bone;

				struct UV {
					int32_t vertexIndex;
					float   value[4];
				} uv;

				struct Material {
					int32_t materialIndex;
					uint8_t mode;
					float   diffuseColor[4];
					float   specularColor[3];
					float   specularExponent;
					float   ambientColor[3];
					float   edgeColor[4];
					float   edgeWeight;
					float   textureTint[4];
					float   environtMapTint[4];
					float   toonTint[4];
				} material;
			};

			std::string         nameJP;
			std::string         nameEN;
			uint8_t             panel;
			uint8_t             type;
			std::vector<Offset> offsets;
		};

		struct Cluster {
			struct Element {
				uint8_t type;
				int32_t index;
			};

			std::string nameJP;
			std::string nameEN;
			uint8_t isSpecial;
			std::vector<Element> elements;
		};

		struct Rigidbody {
			std::string nameJP;
			std::string nameEN;
			int32_t     boneIndex;
			uint8_t     group;
			uint16_t    noCollisionGroup;
			uint8_t     shape;
			float       size[3];
			float       position[3];
			float       rotation[3];
			float       mass;
			float       linearDamping;
			float       angularDamping;
			float       restitution;
			float       friction;
			uint8_t     physicsType;
		};

		struct Joint {
			std::string nameJP;
			std::string nameEN;
			uint8_t     type;
			int32_t     rigidbodyIndices[2];
			float       position[3];
			float       rotation[3];
			float       linearLimit[2][3];
			float       angularLimit[2][3];
			float       linearStiffness[3];
			float       angularStiffness[3];
		};

	public:
		explicit PMXFile(const std::filesystem::path& path);
		const Header& GetHeader() const { return m_header; }
		const Info& GetInfo() const { return m_info; };
		const std::vector<Vertex>& GetVertices() const { return m_vertices; }
		const std::vector<Face>& GetFaces() const { return m_faces; }
		const std::vector<Texture>& GetTextures() const { return m_textures; }
		const std::vector<Material>& GetMaterials() const { return m_materials; }
		const std::vector<Bone>& GetBones() const { return m_bones; }
		const std::vector<Morph>& GetMorphs() const { return m_morphs; }
		const std::vector<Cluster>& GetClusters() const { return m_clusters; }
		const std::vector<Rigidbody>& GetRigidbodies() const { return m_rigidbodies; }
		const std::vector<Joint>& GetJoints() const { return m_joints; }
		const std::string& GetBoneName(uint32_t boneIndex) const { return m_bones[boneIndex].nameJP; }
		const std::string& GetMorphName(uint32_t morphIndex) const { return m_morphs[morphIndex].nameJP; }

	private:
		void ParseText(std::ifstream& stream, std::string& string);
		void ParseHeader(std::ifstream& stream);
		void ParseInfo(std::ifstream& stream);
		void ParseVertex(std::ifstream& stream);
		void ParseFace(std::ifstream& stream);
		void ParseTexture(std::ifstream& stream);
		void ParseMaterial(std::ifstream& stream);
		void ParseBone(std::ifstream& stream);
		void ParseMorph(std::ifstream& stream);
		void ParseFrame(std::ifstream& stream);
		void ParseRigidbody(std::ifstream& stream);
		void ParseJoint(std::ifstream& stream);

	private:
		//std::filesystem::path  m_path;
		Header                 m_header;
		Info                   m_info;
		std::vector<Vertex>    m_vertices;
		std::vector<Face>      m_faces;
		std::vector<Texture>   m_textures;
		std::vector<Material>  m_materials;
		std::vector<Bone>      m_bones;
		std::vector<Morph>     m_morphs;
		std::vector<Cluster>   m_clusters;
		std::vector<Rigidbody> m_rigidbodies;
		std::vector<Joint>     m_joints;
	};

}
