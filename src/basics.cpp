#include <ffge/basics.hpp>
#include <stdexcept>
#include <string>

namespace ffge
{
	void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		std::string error = (type == GL_DEBUG_TYPE_ERROR ? std::string("error: ") : "")
			+ "type = " + std::to_string(type)
			+ ", severity = " + std::to_string(severity)
			+ message
			;
		if (type == GL_DEBUG_TYPE_ERROR)
			throw std::logic_error(error);
	}

    void init()
    {
        glewInit();

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    }
}
