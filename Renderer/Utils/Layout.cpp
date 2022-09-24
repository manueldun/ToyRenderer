#include"Layout.h"
Utils::Layout::Layout(
	const std::vector<AttributeLayout>& attributeLayouts)
	:m_attributeLayouts(attributeLayouts)
{
}



unsigned int Utils::Layout::getIndex(unsigned int inputIndex, unsigned int attributeIndex)const
{
	unsigned int stride = 0;
	for (const auto& attributeLayout : m_attributeLayouts)
	{
		stride += attributeLayout.m_elementCount * attributeLayout.m_bytesPerElement;
	}
	const AttributeLayout attributeLayout = m_attributeLayouts.at(attributeIndex);
	return (inputIndex % attributeLayout.m_elementCount) +
		(inputIndex / attributeLayout.m_elementCount) * stride +
		attributeLayout.m_startOffset;
}


Utils::AttributeLayout::AttributeLayout(
	const std::string name,
	const unsigned int elementCount,
	const unsigned int bytesPerElement,
	const unsigned int startOffset,
	const unsigned int location)
	:m_name(name),
	m_elementCount(elementCount),
	m_bytesPerElement(bytesPerElement),
	m_startOffset(startOffset),
	m_location(location)
{
}
