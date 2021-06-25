#include "Layer.h"

Layer::Layer(std::string _name)
{
	SetName(_name);
}

Layer::Layer()
{
	std::string defName = "Default";
	SetName(defName);
}

Layer::~Layer()
{
}

void Layer::ClearQueues()
{
	m_RenderQueue.clear();
	m_TextRenderQueue.clear();
	m_LineQueue.clear();
	m_FilledBoxes.clear();
	m_WireFrameBoxes.clear();
	//m_RenderTargets.clear();
}

void Layer::AddInterface(RenderInterface& _interface)
{
	m_RenderQueue.push_back(_interface);
}

void Layer::AddText(TextRenderInterface& _textInterface)
{
	m_TextRenderQueue.push_back(_textInterface);
}

void Layer::AddLine(Line& _line)
{
	m_LineQueue.push_back(_line);
}

void Layer::AddFilledBox(FilledBox& _filledBox)
{
	m_FilledBoxes.push_back(_filledBox);
}

void Layer::AddBox(WireFrameBox& _box)
{
	m_WireFrameBoxes.push_back(_box);
}

std::vector<RenderInterface>& Layer::GetRenderQueue()
{
	return m_RenderQueue;
}

std::vector<TextRenderInterface>& Layer::GetTextRenderQueue()
{
	return m_TextRenderQueue;
}

std::vector<Line>& Layer::GetLineQueue()
{
	return m_LineQueue;
}

std::vector<FilledBox>& Layer::GetFilledBoxQueue()
{
	return m_FilledBoxes;
}

std::vector<WireFrameBox>& Layer::GetWireFrameBoxes()
{
	// TODO: insert return statement here
	return m_WireFrameBoxes;
}

std::vector<RenderTarget*>& Layer::GetRenderTargets()
{
	return m_RenderTargets;
}
