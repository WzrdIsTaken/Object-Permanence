#pragma once
#include "EngineCore.h"

#include "entt.h" // Unfortunately not possible (I think?) to forward declare entt in this case, due to pass by value of entt::entity (because under the hood its just a uint32)

#include "MetaECSTypes.h"

// Forward declarations
namespace nabi
{
	struct Context;
} // namespace nabi
namespace nabi::Reflection
{
	class MetaObjectLookup;

	typedef std::unordered_map<std::string, EntityTemplateData> EntityTemplateStore;
	typedef std::unordered_map<std::string, std::vector<EntityData>> EntityGroupStore;
} // namespace nabi::Reflection
namespace pugi
{
	class xml_document;
	class xml_node;
} // namespace pugi

namespace nabi::Reflection
{
	/// <summary>
	/// Does exactly what it says on the tin. Can currently be used to parse route, component and system xml data files.
	/// </summary>
	class XmlParser final
	{
	public:
		/// <summary>
		/// Parses an xml document, determines the type of the data inside and passes it onto the relevant function to handle that data.
		/// Will be called recursivly for nested route docs.
		/// </summary>
		/// <param name="routeDoc">- The first route document, likely containing file paths to other data documents</param>
		/// <param name="registery">- A reference to the registery, forwarded on to the ParseSystems and ParseComponents functions</param>
		/// <param name="systemsLookup">- An optional MetaObjectLookup, which systems will be added to. Pass in nullptr if it is not required</param>
		void ParseXml(std::string_view const routeDoc, nabi::Context& context, MetaObjectLookup* const systemsLookup) NABI_NOEXCEPT;
		/// <summary>
		/// Loads the document at the specified path.
		/// </summary>
		/// <param name="docPath">- The document to load</param>
		/// <returns>The loaded xml document</returns>
		[[nodiscard]] pugi::xml_document LoadDocument(std::string_view const docPath) const NABI_NOEXCEPT;

		/// <summary>
		/// Gets the all of the entity templates that have been read.
		/// Should ideally be called once at the end of xml parsing and used to set up EntityCreator
		/// </summary>
		/// <returns>The entity template store</returns>
		[[nodiscard]] inline EntityTemplateStore const& GetEntityTemplateStore() const NABI_NOEXCEPT { return m_EntityTemplates; };
		/// <summary>
		/// Gets the all of the entity groups that have been read.
		/// Should ideally be called once at the end of xml parsing and used to set up EntityCreator
		/// </summary>
		/// <returns>The entity group store</returns>
		[[nodiscard]] inline EntityGroupStore const& GetEntityGroupStore() const NABI_NOEXCEPT { return m_EntityGroups; };

		/// <summary>
		/// Parses a document containing singleton data. Note: Currently not implemented.
		/// </summary>
		/// <param name="doc">- The singleton data document</param>
		void ParseSingletons(pugi::xml_document const& doc) const NABI_NOEXCEPT;
		/// <summary>
		/// Parses a document containing system data. System documents are designated by c_SystemGroupAttribute's value.
		/// </summary>
		/// <param name="doc">- The system data document</param>
		/// <param name="registery">- A reference to the registery, required to initialise systems</param>
		/// <param name="systemsLookup">- An optional MetaObjectLookup, which systems will be added to. Pass in nullptr if it is not required</param>
		void ParseSystems(pugi::xml_document const& doc, nabi::Context& context, MetaObjectLookup* const systemsLookup) const NABI_NOEXCEPT;
		/// <summary>
		/// Parses a document containing entity data. Component documents are designated by c_EntityGroupAttribute's value.
		/// </summary>
		/// <param name="doc">- The entity data document</param>
		/// <param name="registery">- A reference to the registery, required to assign the component's to the registery</param>
		void ParseEntities(pugi::xml_document const& doc, entt::registry& registery) NABI_NOEXCEPT;

	private:
		/// <summary>
		/// Resolves an entity template and adds it to m_EntityTemplates
		/// </summary>
		/// <param name="entityTemplateNode">- The xml node of the entity template</param>
		void ResolveEntityTemplate(pugi::xml_node const& entityTemplateNode) NABI_NOEXCEPT;
		/// <summary>
		/// Resolves an entity
		/// </summary>
		/// <param name="node">- The xml node of the entity</param>
		/// <param name="entityGroupIdHash">- The group the entity belongs to</param>
		/// <param name="registery">- Reference to the registery</param>
		void ResolveEntity(pugi::xml_node const& node, entt::hashed_string const entityGroupIdHash, entt::registry& registery) NABI_NOEXCEPT;

		/// <summary>
		/// Checks to see if the property exists on any of the passed in components, and if it does overrides its value with the property's value
		/// </summary>
		/// <param name="componentData">- A vector of all the components on an entity or entity template</param>
		/// <param name="propertyNode">- The xml node of a property</param>
		void ResolveEntityComponents(std::vector<ComponentData>& componentData, pugi::xml_node const& propertyNode) const NABI_NOEXCEPT;
		/// <summary>
		/// Checks to see if a child nodes on an entity or entity template is a property or component and resolves it accordingly. 
		/// ResolveEntityComponents is called for properties, for components they are just added to the entity or entity template
		/// </summary>
		/// <param name="entityNode">- The xml node of the entity</param>
		/// <param name="entityComponents">- A reference to the entities or entity template's component data</param>
		void ResolveComponentOrPropertyNode(pugi::xml_node const& entityNode, std::vector<ComponentData>& entityComponents) const NABI_NOEXCEPT;

		/// <summary>
		/// Checks if a node has the c_DebugAttribute attribute and its value is "true"
		/// Debug [systems/entities/components/properties] are only created in debug mode
		/// Then, checks if we are in debug mode. If both these things are true, then the node is debug only
		/// </summary>
		/// <param name="node">- The node to check</param>
		/// <returns>If the node has a debug attribute set to true</returns>
		[[nodiscard]] bool CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(pugi::xml_node const& node) const NABI_NOEXCEPT;

		/// <summary>
		/// Creates a MetaECSTypeData from an xml node
		/// Note - MetaECSTypeData can be a system or component. See MetaECSTypes.h for more infomation
		/// </summary>
		/// <param name="node">- The xml node of the system or component</param>
		/// <returns>The created MetaECSTypeData</returns>
		[[nodiscard]] MetaECSTypeData CreateECSTypeData(pugi::xml_node const& node) const NABI_NOEXCEPT;
		/// <summary>
		/// Creates a PropertyData from an xml node
		/// </summary>
		/// <param name="node">- The xml node of the property</param>
		/// <returns>The created PropertyData</returns>
		[[nodiscard]] PropertyData CreatePropertyData(pugi::xml_node const& node) const NABI_NOEXCEPT;

		/// <summary>
		/// Checks to see if an entity group already exists in the store. If it does, add the entity data to it. Else first create a new group
		/// </summary>
		/// <param name="docGroup">- The name of the entity group to add the entityData to</param>
		/// <param name="entityGroup">- Reflected data about an entity which will be used in its construction</param>
		void AddEntityTemplateToEntityGroupStore(std::string const& docGroup, EntityData const& entityData) NABI_NOEXCEPT;

		// Entity Template Stuff
		EntityTemplateStore m_EntityTemplates; // (template name - template)
		EntityGroupStore m_EntityGroups; // (entity group name - vector of entity data)  

		// File Specifiers 
		std::string const c_RouteDocAttribute = "route";
		std::string const c_SingletonGroupAttribute = "singleton_group";
		std::string const c_SystemGroupAttribute = "system_group";
		std::string const c_EntityGroupAttribute = "entity_group";

		// Types
		std::string const c_SingletonAttribute = "singleton";
		std::string const c_SystemAttribute = "system";
		std::string const c_EntityAttribute = "entity";
		std::string const c_EntityTemplateAttribute = "entity_template";

		std::string const c_ComponentAttribute = "component";
		std::string const c_PropertyAttribute = "property";

		// Properties
		std::string const c_TemplateAttribute = "template";
		std::string const c_DelayCreationAttribute = "delay_creation";

		std::string const c_IdAttribute = "id";
		std::string const c_ValueAttribute = "value";

		// Debug
		std::string const c_DebugAttribute = "debug";
	};
} // namespace nabi::Reflection
