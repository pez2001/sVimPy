"""
 * sVimPy - small Virtual interpreting machine for Python
 * (c) 2012 by Tim Theede aka Pez2001 <pez2001@voyagerproject.de> / vp
 *
 * python vm api wrapper
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
"""
class Vm:
	def AddGlobal(key,new_global):pass
	def RemoveGlobal(key):pass
	def GetGlobal(key):pass
	def Interrupt():pass
	def Continue():pass
	def Exit(message,err_no):pass
	def Stop():pass
	def RunPYC():pass
	def RunRPYC():pass
	def RunRPYCPlus():pass
	def RunFunction():pass
	def RunObject():pass
	def StartMethod():pass
	def RunMethod():pass
	def StartObject():pass
	def ClearGC():pass
	def DumpObject(obj):pass
	def CopyObject(obj):pass
