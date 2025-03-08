
#pragma once

#ifndef __IC3_TOOLS_ICE_LOG_VIEWER_APPLICATION_H__
#define __IC3_TOOLS_ICE_LOG_VIEWER_APPLICATION_H__

#include <QApplication>

#include <Ic3/System/Core/DisplayCommon.h>
#include <Ic3/System/Core/Frame.h>

namespace Ic3::Tools::IceLogViewer
{

	using namespace Ic3::System;

	class Application : public QApplication
	{
	public:
		Application( int pArgc, char ** pArgv );
		virtual ~Application();

		CPPX_ATTR_NO_DISCARD QSize GetDisplaySize() const noexcept;

	private:
		QScreen * _qtAppScreen = nullptr;
	};

}

#endif // __IC3_TOOLS_ICE_LOG_VIEWER_APPLICATION_H__
