#pragma once
#include<vector>
#include<string>
namespace Utils
{
	class AttributeLayout {
	public:
		AttributeLayout(
			const std::string name,
			const unsigned int elementCount,
			const unsigned int bytesPerElement,
			const unsigned int startOffset,
			const unsigned int location
		);
		const std::string m_name;
		const unsigned int m_elementCount;
		const unsigned int m_bytesPerElement;
		const unsigned int m_startOffset;
		const unsigned int m_location;

	};
	class Layout {
	public:
		Layout(const std::vector<AttributeLayout>& attributeLayouts);

		unsigned int getIndex(unsigned int inputIndex, unsigned int attributeIndex)const;

		const std::vector<AttributeLayout> m_attributeLayouts;

	};


	const Layout standardLayout = Layout({
		AttributeLayout("Position",3,4,0,0),
		AttributeLayout("Normal",3,4,3,2),
		AttributeLayout("TexCoord",2,4,6,1), });
	const Layout tangentLayout = Layout({
		AttributeLayout("Position",3,4,0,0),
		AttributeLayout("Normal",3,4,3,2),
		AttributeLayout("TexCoord",2,4,6,1),
		AttributeLayout("Tangent",4,4,8,3)
		});
}