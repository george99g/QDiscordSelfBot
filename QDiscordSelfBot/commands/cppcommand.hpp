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

#ifndef CPPCOMMAND_HPP
#define CPPCOMMAND_HPP

#include <QObject>
#include <QProcess>
#include <QDir>
#include <QTemporaryDir>
#include <QFile>
#include <QPointer>
#include "command.hpp"
#include "config.hpp"

class CppCommand : public QObject, public Command
{
public:
	explicit CppCommand(QDiscord& discord, Config& config, QObject* parent = 0);
	void dispatch(QDiscordMessage message, QStringList args);
	QString commandName();
	QString helpText();
	QStringList argumentText();
private:
	void gppFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void appFinished(int exitCode, QProcess::ExitStatus exitStatus);
	Config& _config;
	bool _processing;
	QDiscordMessage _currentMessage;
	QTemporaryDir _tempDir;
	QProcess _gpp;
	QProcess _app;
};

#endif // CPPCOMMAND_HPP
