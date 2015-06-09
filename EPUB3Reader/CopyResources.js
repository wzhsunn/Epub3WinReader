/*
 *  MakeHeaders.js
 *  ePub3
 *
 *  Created by Jim Dovey on 2013-04-29.
 *  Copyright (c) 2012-2013 The Readium Foundation and contributors.
 *  
 *  The Readium SDK is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// paths are automatically relative to the location of this script file
var destDir = "../Scripts";

var fso = new ActiveXObject("Scripting.FileSystemObject");
var srcShared = fso.GetFolder("../readium-shared-js").Path;
var srcScripts = fso.GetFolder("ReaderScripts").Path;
var sharedGroups = {
    "shared" : ["js\\*.js", "js", "lib", "static"]
};

var scriptsGroups = {
    "scripts" : ["*.epub", "*.js", "*.sh", "*.epub", "*.css", "*.html", "mathjax"]
};
if (!fso.FolderExists(destDir))
    fso.CreateFolder(destDir);

for (var groupName in sharedGroups)
{    
   var outFolder = fso.BuildPath(destDir, "");
   if (!fso.FolderExists(outFolder))
		fso.CreateFolder(outFolder);
   outFolder += "\\";

	WScript.Echo("Copying from '" + groupName + "'" + ' to ' + destDir);

    var inputs = sharedGroups[groupName];
    for (var i in inputs)
    {
        var path = fso.BuildPath(srcShared, inputs[i]);
        WScript.Echo("Copying from '" + path + "'" + ' to ' + outFolder);
        try {
            fso.CopyFolder(path, outFolder, true);  // may fail if path points to files
        } catch (e) {
        }
        try {
            fso.CopyFile(path, outFolder, true);    // may fail if path points to folders
        } catch (e) {
        }
    }
}

for (var groupName in scriptsGroups)
{
    var outFolder = fso.BuildPath(destDir, "");
    if (!fso.FolderExists(outFolder))
        fso.CreateFolder(outFolder);
    outFolder += "\\";

	WScript.Echo("Copying from '" + groupName + "'" + ' to ' + outFolder);

    var inputs = scriptsGroups[groupName];
    for (var i in inputs)
    {
        var path = fso.BuildPath(srcScripts, inputs[i]);
        WScript.Echo("Copying from '" + path + "'" + ' to ' + outFolder);
        try {
            fso.CopyFolder(path, outFolder, true);  // may fail if path points to files
        } catch (e) {
        }
        try {
            fso.CopyFile(path, outFolder, true);    // may fail if path points to folders
        } catch (e) {
        }
    }
}

