#!/usr/bin/env python

############################################################################
# LGPL License                                                             #
#                                                                          #
# This file is part of the Machine Learning Framework.                     #
# Copyright (c) 2010-2012, Philipp Kraus, <philipp.kraus@flashpixx.de>     #
# This program is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU Lesser General Public License as           #
# published by the Free Software Foundation, either version 3 of the       #
# License, or (at your option) any later version.                          #
#                                                                          #
# This program is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU Lesser General Public License for more details.                      #
#                                                                          #
# You should have received a copy of the GNU Lesser General Public License #
# along with this program. If not, see <http://www.gnu.org/licenses/>.     #
############################################################################


# -*- coding: utf-8 -*-

import os
import sys
import imp
import time
import glob
import hashlib
import clang.cindex

# get LLVM & CLang from http://llvm.org/releases/ , extract both and rename clang source dir to "clang" and copy it to LLVM source dir/tools



#=== main functions for creating and parsing AST =====================================================================================

# function for getting variable / namespace type
# @param node AST node
# @return list with used datatypes
def getTypeList( node ) :
    names = []
    types = []

    for i in node.get_children() :
        if i.kind == clang.cindex.CursorKind.CALL_EXPR or \
           i.kind == clang.cindex.CursorKind.UNEXPOSED_EXPR :
            continue
           
        if i.displayname :
            names.append( str(i.displayname).strip() )
            types.append( i.kind )

    # build types for template variables build
    lst = []
    l   = 0
    for n, i in enumerate(types) :
        if i == clang.cindex.CursorKind.TEMPLATE_REF or \
           i == clang.cindex.CursorKind.TYPE_REF :
            lst.append( "::".join(names[l:n+1]) )
            l = n+1

    # remove class / struct / enum prefixes and split on space and return only last element
    if len(lst) > 0 :
        f = filter(lambda x: "class" in x or "enum" in x or "struct" in x, lst)
        if len(f) > 1:
            lst = [lst[-1]]
        for n in ["struct", "class", "enum"] :
            lst = [ (i.replace(n, "").split(" ")[-1]).strip() for i in lst ]
            
    if not lst or len(lst) == 0 :
        return None
    elif len(lst) == 1 :
        return lst[0]
    return lst


# create a full namespace
# @param node namespace alias node
# @return string with namespace
def getNamespaceList( node ) :
    lst = []
    for i in node.get_children() :
        if i.displayname :
            lst.append( str(i.displayname) )
    if len(lst) > 0 :
        return "::".join(lst)
    return None


# adds a message into the message dict
# @param messages dict with messages
# @param filename filename
# @param lineno linenumber
# @param msgtype message type
# @param msg message or message list
def addMessage( messages, filename, lineno, msgtype, msg ) :
    if not (msg and msgtype) :
        return
        
    if type([]) <> type(msg) :
        msg = [msg]
    if type([]) <> type(msgtype) :
        msgtype = [msgtype]
    if len(msgtype) <> len(msg) :
        raise Exception("message types and messages are not equal")
        
    for n,t in zip(msg, msgtype) :
        if not t in messages :
            messages[t] = {}
        if not filename in messages[t] :
            messages[t][filename] = {}
        if not lineno in messages[t][filename] :
            messages[t][filename][lineno] = {}
    
        h = hashlib.sha1()
        h.update(n)
        if not h.hexdigest() in messages[t][filename][lineno] :
            messages[t][filename][lineno][h.hexdigest()] = n


# calls the message function on the node value
# @param rules Python modul with check funtions
# @param node AST node
# @param parentnode AST parent node
# @param source list with files, that should be parsed
# @param messages message dict
# @param returns 0 or 1 for the LLOC metric
def treeNode( rules, node, parentnode, source, messages ) :

    # we need the check of the file here, because on extern includes
    # the full file is found in the AST, so we check only nodes, which
    # file is defined in the command line parameter, otherwise we ignore it
    filename = os.path.normpath(str(node.location.file))
    if not filename in source :
        return 0 
        
    lineno  = int(node.location.line)
    msg     = None
    msgtype = None

    if  hasattr(rules, "check_Namespace") and callable(getattr(rules, "check_Namespace")) and node.kind == clang.cindex.CursorKind.NAMESPACE :
        msgtype, msg = rules.check_Namespace(filename, lineno, node.spelling)
        
    if hasattr(rules, "check_NamespaceAlias") and callable(getattr(rules, "check_NamespaceAlias")) and node.kind == clang.cindex.CursorKind.NAMESPACE_ALIAS :
        msgtype, msg = rules.check_NamespaceAlias(filename, lineno, node.spelling, getNamespaceList(node))
        
    elif hasattr(rules, "check_Class") and callable(getattr(rules, "check_Class")) and node.kind == clang.cindex.CursorKind.CLASS_DECL :
        msgtype, msg = rules.check_Class(filename, lineno, node.spelling)
    
    elif hasattr(rules, "check_Method") and callable(getattr(rules, "check_Method")) and node.kind == clang.cindex.CursorKind.CXX_METHOD :
        msgtype, msg = rules.check_Method(filename, lineno, node.spelling, node.displayname)
    
    elif hasattr(rules, "check_Ctor") and callable(getattr(rules, "check_Ctor")) and node.kind == clang.cindex.CursorKind.CONSTRUCTOR :
        msgtype, msg = rules.check_Ctor(filename, lineno, node.spelling, node.displayname)
    
    elif hasattr(rules, "check_Dtor") and callable(getattr(rules, "check_Dtor")) and node.kind == clang.cindex.CursorKind.DESTRUCTOR :
        msgtype, msg = rules.check_Dtor(filename, lineno, node.spelling)
    
    elif hasattr(rules, "check_FunctionTemplate") and callable(getattr(rules, "check_FunctionTemplate")) and node.kind == clang.cindex.CursorKind.FUNCTION_TEMPLATE :
        msgtype, msg = rules.check_FunctionTemplate(filename, lineno, node.spelling, node.displayname)
    
            
    elif hasattr(rules, "check_MemberVar") and callable(getattr(rules, "check_MemberVar")) and node.kind == clang.cindex.CursorKind.FIELD_DECL :
        msgtype, msg = rules.check_MemberVar(filename, lineno, node.displayname, getTypeList(node))
    
    elif hasattr(rules, "check_ParameterVar") and callable(getattr(rules, "check_ParameterVar")) and node.kind == clang.cindex.CursorKind.PARM_DECL and node.spelling :
        msgtype, msg = rules.check_ParameterVar(filename, lineno, node.spelling, getTypeList(node))
    
    elif hasattr(rules, "check_LocalVar") and callable(getattr(rules, "check_LocalVar")) and node.kind == clang.cindex.CursorKind.VAR_DECL and parentnode and not parentnode.kind == clang.cindex.CursorKind.CLASS_DECL:
        msgtype, msg = rules.check_LocalVar(filename, lineno, node.spelling, getTypeList(node))
    
    elif hasattr(rules, "check_TemplateVar") and callable(getattr(rules, "check_TemplateVar")) and node.kind == clang.cindex.CursorKind.TEMPLATE_TYPE_PARAMETER :
        msgtype, msg = rules.check_TemplateVar(filename, lineno, node.spelling)

    addMessage( messages, filename, lineno, msgtype, msg )
    return 1


# recursiv function for parsing the tree
# @param rules Python modul with check funtions
# @param node AST node
# @param parentnode AST parent node
# @param source list with files, that should be parsed
# @param messages list with error messages
# @return lloc metric for "logical lines of code"
def treeParse( rules, node, parentnode, source, messages ) :
    lloc = 0
    lloc = treeNode(rules, node, parentnode, source, messages)
 
    if node.kind.is_reference() : 
        lloc = lloc + treeNode(rules, node, parentnode, source, messages)

    for i in node.get_children() :
        # ignore some elements
        if i.kind == clang.cindex.CursorKind.CALL_EXPR :
            continue
        lloc = lloc + treeParse(rules, i, node, source, messages)
    return lloc


# main function for checking the AST
# @param rules Python modul with check funtions
# @param source source files / list
# @param printtree boolean flag for printing the AST
# @param includedir directory list for includes
# @param defines define list, that should be set
def stylecheck( rules, source, printtree = False, includedir = [], defines = [] ) :
    index    = clang.cindex.Index.create()
    messages = {}
    
    if type([]) <> type(source) :
        source = [source]
        
    # normalize all path and add paths to include parameter
    source = [os.path.normpath(i) for i in source]
    arglst = ["-x", "c++"]
    for i in includedir :
        arglst.extend(["-I", i])
    for i in defines :
        arglst.extend(["-D", i])
        
    # metric counter
    lloc = 0
        
    for i in source :
    
        # read the file first and call into function
        fp = open( i, "r" )
        filecontent = fp.read()
        fp.close()
        if rules and hasattr(rules, "check_Raw") and callable(getattr(rules, "check_Raw")) :
            msgtype, msg = rules.check_Raw(i, filecontent)
            addMessage( messages, i, 0, msgtype, msg )

        # get only parse elements, that are relative to the project directory
        tu  = index.parse(str(i), args=arglst)
        for j in tu.cursor.get_children() :
            if printtree :
                print_cursor_and_children(j, source)
            else :
                lloc = lloc + treeParse(rules, j, None, source, messages)
            
    # print all messages
    stop    = False
    stoplst = None
    if rules and hasattr(rules, "abort") and callable(getattr(rules, "abort")) :
        stoplst = rules.abort()
    if type([]) <> type(stoplst) :
        stoplst = [stoplst]
    
    for msgtype, files in messages.iteritems() :
        if msgtype in stoplst or "all" in stoplst :
            stop = True
    
        head = "=== style guide check [%s] " % (msgtype)
        print head + "="*(80-len(head))
        for i, (file, lines) in enumerate(files.items()) :
            print "file [%s]" % (file)
            
            for j, (line, msgs) in enumerate(lines.items()) :
                if line == 0 :
                    print print_indent(1) + "global"
                else :
                    print print_indent(1) + "line [%s]" % (line)
                
                for msg in msgs.values() :
                    print print_indent(2) + msg
                if j <> len(lines)-1 :
                    print
                    
            if i <> len(files)-1 :
                print "-"*80
        print "="*80
        
    print "\nLLOC (Logical Lines of Code) metric [%s]: %d" % (i, lloc)
    if stop :
        raise Exception("===>> style guide check fails (see log) <<===")
#=====================================================================================================================================




#=== function for printing the AST ===================================================================================================

# function for print ident
# @param level tree deep
# @return string
def print_indent(level) :
    return "    "*level


# function for output of a node
# @param cursor node
# @param level tree deep
def print_node(cursor, level) :
    spelling = ""
    displayname = ""

    if cursor.spelling :
        spelling = "\t" + str(cursor.spelling)
    if cursor.displayname :
        displayname = "\t<" + str(cursor.displayname) + ">"
    if str(cursor.spelling) == str(cursor.displayname) :
        displayname = ""
        
    print print_indent(level) + str(cursor.kind).replace("CursorKind.", "") + spelling + displayname


# prints the tree recursiv
# @param cursor root node
# @param source list of files which should be parsed
def print_cursor_and_children(cursor, source, level=0) :
    if not os.path.normpath(str(cursor.location.file)) in source :
        return

    print_node(cursor, level)
    if cursor.kind.is_reference():
        print print_indent(level) + "reference to :"
        print_node(cursor, level+1)
        print

    has_children = False
    for c in cursor.get_children():
        if not has_children:
            print print_indent(level) + "{"
            has_children = True
        print_cursor_and_children(c, source, level+1)

    if has_children:
        print print_indent(level) + "}"
#=====================================================================================================================================






     
#=== main calls ======================================================================================================================
if __name__ == "__main__":

    if len(sys.argv) < 2 :
        raise Exception("no arguments are set")
    else :
        # split files and dirs (dirs are set for include)
        files   = []
        include = []
        defines = []

        printtree = False
        arglst    = sys.argv[2:]
        rules     = None
        if sys.argv[1] == "print" :
            printtree = True
        elif sys.argv[1] == "help" :
            print "first parameter:\tprint for printing the abstract syntax tree"
            print "\t\t\tpython file with check rules"
            print
            print "-I\t\t\tdirectory prefix for include directories"
            print "-D\t\t\tprefix for compiler defines"
            print "-X\t\t\tprefix for directory ignore"
            print
            print "-ENV\t\t\tflag for using environment variables"
            print "-TIME\t\t\shows execution time"
            print
            print "all other directories parameter can be files or directories which are scanned for C/C++ files"
            sys.exit
        else :
            rules  = imp.load_source("rules", sys.argv[1])

        try :
            ignorelst = [i[2:] for i in arglst if i.startswith("-X")]   
            starttime = time.time()
            if "-ENV" in arglst :
                if "CPPPATH" in os.environ :
                    include.extend(os.environ["CPPPATH"].split(os.pathsep))
                if "CPATH" in os.environ :
                    include.extend(os.environ["CPATH"].split(os.pathsep))
                
            for i in arglst :

                if i.startswith("-X") or i.startswith("-ENV") or i.startswith("-TIME") :
                    pass
            
                elif i.startswith("-I") :
                    dir = i[2:]
                    if not os.path.exists(dir) or not os.path.isdir(dir) :
                        raise Exception("include directory ["+dir+"] is not found")
                    include.append(dir)
                    
                elif i.startswith("-D") :
                    defines.append(i[2:])
                    
                elif os.path.exists(i) and os.path.isdir(i) :
                    for root, dirnames, filenames in os.walk(i[:-1]) :
                        if any([i in root for i in ignorelst]) :
                            continue
                        files.extend(glob.glob( os.path.join(root, "*.cpp") ))
                        files.extend(glob.glob( os.path.join(root, "*.hpp") ))
                        files.extend(glob.glob( os.path.join(root, "*.h") ))

                elif os.path.exists(i) and os.path.isfile(i) and (i.endswith(".cpp") or i.endswith(".hpp") or i.endswith(".h")):
                    files.append(i)
                    
                else :
                    raise Exception("file / directory ["+i+"] is not found / is incorrect (no C/C++ file)")
            
            stylecheck( rules, files, printtree, set(include), set(defines) )
            if "-TIME" in arglst :
                print "execution time: " + str(time.time() - starttime) + "sec"
        
        except Exception as e :
            print e
            if "-TIME" in arglst :
                print "\nexecution time: " + str(time.time() - starttime) + "sec"
            sys.exit(1)
                    
#=====================================================================================================================================
