module crude.system.keyboard;

namespace crude::system
{

Keyboard_Event_Handler::Keyboard_Event_Handler()
  :
  m_autoRepeatKeys(false),
  m_autoRepeatChars(false)
{
	m_keyState.fill(false);
}

bool Keyboard_Event_Handler::keyIsPressed(const core::uint8 keycode) const
{
	return m_keyState[keycode];
}

bool Keyboard_Event_Handler::keyBufferIsEmpty() const
{
	return m_keyBuffer.empty();
}

bool Keyboard_Event_Handler::charBufferIsEmpty() const
{
	return m_charBuffer.empty();
}

Keyboard_Event Keyboard_Event_Handler::readKey()
{
	if (m_keyBuffer.empty())
	{
		return Keyboard_Event();
	}
	else
	{
		Keyboard_Event event = m_keyBuffer.front();
		m_keyBuffer.pop();
		return event;
	}
}

core::uint8 Keyboard_Event_Handler::readChar()
{
	if (m_charBuffer.empty())
	{
		return 0u;
	}
	else
	{
    core::uint8 symbol = m_charBuffer.front();
		m_charBuffer.pop();
		return symbol;
	}
}

void Keyboard_Event_Handler::onKeyPressed(const core::uint8 key)
{
	m_keyState[key] = true;
	m_keyBuffer.push(Keyboard_Event(KEYBOARD_EVENT_TYPE_PRESS, key));
}

void Keyboard_Event_Handler::onKeyReleased(const core::uint8 key)
{
	m_keyState[key] = false;
	m_keyBuffer.push(Keyboard_Event(KEYBOARD_EVENT_TYPE_RELEASE, key));
}

void Keyboard_Event_Handler::onChar(const uint8 key)
{
	m_charBuffer.push(key);
}

void Keyboard_Event_Handler::enableAutoRepeatKeys()
{
	m_autoRepeatKeys = true;
}

void Keyboard_Event_Handler::disableAutoRepeatKeys()
{
	m_autoRepeatKeys = false;
}

void Keyboard_Event_Handler::enbaleAutoRepeatChars()
{
	autoRepeatChars = true;
}

void Keyboard_Event_Handler::desableAutoRepeatChars()
{
	m_autoRepeatChars = false;
}

bool Keyboard_Event_Handler::isKeyAutoRepeat() const
{
	return m_autoRepeatKeys;
}

bool Keyboard_Event_Handler::isCharsAutoRepeat() const
{
	return m_autoRepeatChars;
}

}
