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
import re


#=== syntax check rules ==============================================================================================================

# function that returns a messagetype or list
# of messagetypes which raises an exception 
# if one messagetype occurs
# @return messagetyp / list / 'all' for stop
def abort() :
    return "error"





# function for checking the raw content of a file
# @param filename filename (and path) of the file
# @param content filecontent
# @param tuple with messagetype and message
def check_Raw( filename, content ) :
    msg     = []
    msgtype = []
    
    # last line must be a blank line
    if not content.endswith("\n") :
        msgtype.append("warning")
        msg.append( "file [%s] must be end with a blank line" % (filename) )

    #if "\t" in content :
    #    msgtype.append("warning")
    #    msg.append("tabulator must replace with '\\t' or spaces")
    
    
    # remove dots on the beginning of the filename and
    # split it on the file seperator
    while filename.startswith(".") :
        filename = filename[1:]
    filepart = filter(None, filename.split(os.path.sep))

    for i in filepart :
        if not re.match( r'[a-z]+', i ) :
            msgtype.append("error")
            msg.append("filename and directories must be only lowercase letters, [%s] is not correct" % (i))

    # check ifdef & define name
    if (filename.endswith(".h") or filename.endswith(".hpp")) and not filename.endswith(os.path.join("tools", "language", "iso639.h")) :
    
        if filename.endswith("machinelearning.h") :
            defname = "__MACHINELEARNING_H"
        else :
            defname = "_".join(filepart).replace(".", "_").upper()
            defname = "__MACHINELEARNING_" + defname
            
        if not re.search( r'#ifndef '+defname, content ) and not re.search( r'(.*)!defined '+defname, content ) :
            msgtype.append("error")
            msg.append("header ifndef or !definied must be [%s]" % (defname))
        if not re.search( r'#define '+defname, content ) :
            msgtype.append("error")
            msg.append("header define must be [%s]" % (defname))
            
    return msgtype, msg



# function for check namespaces
# @param filename filename (and path) of the file
# @param lineno line number
# @param name namespace string
# @param tuple with messagetype and message
def check_Namespace( filename, lineno, name ) :
    if not re.match( r'[a-z]+', name ) :
        return "error", "namespaces must be only lowercase letters, [%s] is not correct" % (name)
    return None, None
    


# function for check namespaces alias
# @param filename filename (and path) of the file
# @param lineno line number
# @param name namespace alias
# @param full full namespace signature
# @param tuple with messagetype and message
def check_NamespaceAlias( filename, lineno, name, full ) :
    if not re.match( r'[a-z]+', name ) :
        return "error", "namespaces alias must be only lowercase letters, [%s / %s] is not correct" % (name, full)
    return None, None


    
# function for check classes
# @param filename filename (and path) of the file
# @param lineno line number
# @param name class string
# @param tuple with messagetype and message
#def check_Class( filename, lineno, name ) :
#    return None, None
    
    
    
# function that checks methods
# @param filename filename (and path) of the file
# @param lineno line number
# @param name name of the method
# @param full full method signature
# @param tuple with messagetype and message
def check_Method( filename, lineno, name, full ) :
    if name == "BOOST_STATIC_ASSERT" :
        return None, None
    
    elif not re.match( r'[a-z][a-zA-Z_0-9]+', name ) :
        return "error", "method name must be start with an lowercase letter and followed by letters, numbers or underscore, [%s] is not correct" % (name)
    return None, None
    
    
    
# function that checks constructors
# @param filename filename (and path) of the file
# @param lineno line number
# @param name name of the method
# @param full full method signature
# @param tuple with messagetype and message
#def check_Ctor( filename, lineno, name, full ) :
#    return None, None
    
    
    
# function for checking destructors
# @param filename filename (and path) of the file
# @param lineno line number
# @param name name of the method
# @param tuple with messagetype and message
#def check_Dtor( filename, lineno, name ) :
#    return None, None
    


# function for check of function templates
# @param filename filename (and path) of the file
# @param lineno line number
# @param name name of the method
# @param full full method signature
# @param tuple with messagetype and message
#def check_FunctionTemplate( filename, lineno, name, full ) :
#    return None, None   
    
    
    
    
    
# function for check member variables
# @param filename filename (and path) of the file
# @param lineno line number
# @param name name of the member
# @param typelst of the variable (list or string)
# @param tuple with messagetype and message
def check_MemberVar( filename, lineno, name, typelst ) :
    if filename.endswith(os.path.join("geneticalgorithm","population.hpp")) and \
       (name == "distribution" or name == "probabilityvalue" or name == "first" or name == "second" or name == "third") :
        return None, None

    if not name.startswith("m_") :
        if name.startswith("m") :
            return "warning", "member variable [%s] with type [%s] begins only with 'm', but should begin with 'm_'" % (name, typelst)
        return "error", "member variables must begin with 'm_', but [%s] with type [%s] is incorrect" % (name, typelst)
    return None, None



# function for check parameter variables
# @param filename filename (and path) of the file
# @param lineno line number
# @param name name of the parameter
# @param typelst of the variable (list or string)
# @param tuple with messagetype and message
def check_ParameterVar( filename, lineno, name, typelst ) :
    if not name.startswith("p_") :
        if name.startswith("p") :
            return "warning", "parameter variable [%s] with type [%s] begins only with 'p', but should begin with 'p_'" % (name, typelst)
        return "error", "parameter variables must begin with 'p_', but [%s] with type [%s] is incorrect" % (name, typelst)
    return None, None
    


# function for check local variables
# @param filename filename (and path) of the file
# @param lineno line number
# @param name name of the local variable
# @param typelst of the variable (list or string)
# @param tuple with messagetype and message
def check_LocalVar( filename, lineno, name, typelst ) :
    # check special variables
    if filename.endswith("machinelearning.cpp") :
        if name == "m_random" and "machinelearning::tools::random" in typelst :
            return None, None
            
    if filename.endswith(os.path.join("examples", "sources", "newsgroup.cpp")) and name == "it" :
        return None, None
        
    if "foreach" in name :
        return None, None


    # check iterator variables (we check the types for iterator types or
    # std::map, std::vector or std::multimap, because in some case the AST does not shown the iterator type)
    if name == "it" :
        if type(typelst) == type([]) and len(filter(lambda x: "iterator" in x, typelst)) > 0 :
            return None, None
        elif type(typelst) == type("") and "iterator" in typelst :
            return None, None
        elif type(typelst) == type([]) and len(filter(lambda x: "std::map" in x or "std::vector" in x or "std::multimap" in x, typelst)) > 0 :
            return None, None

    # check all other variables
    if name == "i" or name == "l" or name == "n" or name == "j" :
        return None, None
    if not name.startswith("l_") :
        if name.startswith("l") :
            return "warning", "local variable [%s] with type [%s] begins only with 'l', but should begin with 'l_'" % (name, typelst)
        return "error", "local variables must begin with 'l_' or named 'i', 'j', 'n', 'l' or 'it' (for iterators), but [%s] with type [%s] is incorrect" % (name, typelst)
    return None, None
    
    
    
# function for check template variables
# @param filename filename (and path) of the file
# @param lineno line number
# @param name name of the template variable
# @param tuple with messagetype and message
def check_TemplateVar( filename, lineno, name ) :
    if not re.match( r'[A-Z]+', name ) :
        return "error", "template variable must be only uppercase letters, [%s] is not correct" % (name)
    return None, None

#=====================================================================================================================================
