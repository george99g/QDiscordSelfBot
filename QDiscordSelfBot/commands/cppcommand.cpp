/*
 * QDiscordSelfBot - An example self-bot written using the QDiscord library.
 * Copyright (C) 2017 george99g
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.	 If not, see <http://www.gnu.org/licenses/>.
 */

#include "cppcommand.hpp"

CppCommand::CppCommand(QDiscord& discord, Config& config, QObject* parent)
:QObject(parent), Command(discord), _config(config), _processing(false)
{
	//Again, caching.
	QJsonObject configObject = _config.object();
	if(!configObject.contains("cpp"))
	{
		configObject["cpp"] = QJsonObject({{"compiler_path", "/usr/bin/g++"}});
		_config.setObject(configObject);
	}
	_gpp.setProgram(
				configObject["cpp"].toObject()["compiler_path"].toString("/usr/bin/g++")
			);
	connect(&_gpp, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
			this, &CppCommand::gppFinished);
	connect(&_app, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
			this, &CppCommand::appFinished);

}

void CppCommand::dispatch(QDiscordMessage message, QStringList args)
{
	QFile gppExecutable(_gpp.program());
	if(!gppExecutable.exists())
	{
		QString outputText = _gpp.program();
		outputText += " does not seem to be a valid file. Set the "
				"cpp.compiler_path config file variable to a valid compiler "
				"path.";
		_discord.rest()->editMessage(message, outputText);
		return;
	}
	//Check if we're already compiling/executing something, so we can avoid
	//breaking things.
	if(_processing)
	{
		_discord.rest()->editMessage(message, "Already processing.");
		return;
	}
	//All commands get access to the message, and thus to the entire raw
	//content. This allows them to implement their own parsing if they wish.
	//In this case, this is abused to get the code out.
	QString code = message.content().mid(message.content().indexOf("\n") + 1);
	if(!code.startsWith("```cpp") || !code.endsWith("```"))
	{
		_discord.rest()->editMessage(message, "Code formatting error.");
		return;
	}
	int i = QString("```cpp\n").length();
	code = code.mid(i, code.mid(i).indexOf("```") - 1);
	//Open a file in our temporary directory so we can dump the source code
	//inside.
	QFile file(_tempDir.path() + "/main.cpp");
	if(!file.open(QFile::WriteOnly | QFile::Truncate))
	{
		_discord.rest()->editMessage(message, "Could not open temporary file.");
		return;
	}
	file.write((code + "\n").toUtf8());
	file.flush();
	file.close();
	//Add some of our own arguments to the compiler so it knows where our source
	//is and what our executable should be named.
	args.append(file.fileName());
	args.append("-o");
	args.append(_tempDir.path() + "/main.exe");
	//Save the message so we remember which channel we're supposed to send to.
	_currentMessage = message;
	//Block any further calls until we're done.
	_processing	= true;
	_gpp.setArguments(args);
	_discord.rest()->sendMessage("Compiling...", message.channelId());
	_gpp.start();
	//Kill the process after 3 seconds in case someone decides to put the
	//compiler into an infinite loop with templates.
	QTimer::singleShot(3000,
	[this]() {
		if(_gpp.state() == QProcess::Running)
			_gpp.kill();
	});
}

QString CppCommand::commandName()
{
	return "cpp";
}

QString CppCommand::helpText()
{
	return "Compiles and executes C++ code.";
}

QStringList CppCommand::argumentText()
{
	return {"[ARGS]\\n(CODE)",
		"ARGS are the arguments to pass to the compiler.",
		"CODE is the code."};
}

void CppCommand::gppFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus);
	if(exitCode != EXIT_SUCCESS)
	{
		_discord.rest()->sendMessage(
						"Compile failed:\n```\n" + _gpp.readAllStandardOutput() + _gpp.readAllStandardError() + "```",
						_currentMessage.channelId()
					);
		_processing = false;
		return;
	}
	_gpp.readAll();
	_app.setProgram(_tempDir.path() + "/main.exe");
	_discord.rest()->sendMessage("Running...", _currentMessage.channelId());
	_app.start();
	QTimer::singleShot(3000,
	[this]() {
		if(_app.state() == QProcess::Running)
			_app.kill();
	});
}

void CppCommand::appFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitCode);
	Q_UNUSED(exitStatus);
	QString output = "Output:\n```";
	output += _app.readAll();
	output += "```";
	if(output.length() > 2000)
	{
		_discord.rest()->sendMessage("Output too large.", _currentMessage.channelId());
		_processing = false;
		return;
	}
	_discord.rest()->sendMessage(output, _currentMessage.channelId());
	_processing = false;
}
