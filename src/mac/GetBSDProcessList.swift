/**
* Copyright (c) 2015-2017 Parity Technologies (UK) Ltd.
* Copyright (c) 2016-2017 metaverse core developers (see MVS-AUTHORS)
*
* This file is part of metaverse.app.
*
* metaverse-explorer is free software: you can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License with
* additional permissions to the one published by the Free Software
* Foundation, either version 3 of the License, or (at your option)
* any later version. For more information see LICENSE.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// Based on https://github.com/soh335/GetBSDProcessList

import Foundation
import Darwin

public func GetBSDProcessList() -> ([kinfo_proc]?)  {
	
	var done = false
	var result: [kinfo_proc]?
	var err: Int32
	
	repeat {
		let name = [CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0];
		let namePointer = name.withUnsafeBufferPointer { UnsafeMutablePointer<Int32>(mutating: $0.baseAddress) }
		var length: Int = 0
		
		err = sysctl(namePointer, u_int(name.count), nil, &length, nil, 0)
		if err == -1 {
			err = errno
		}
		
		if err == 0 {
			let count = length / MemoryLayout<kinfo_proc>.stride
			result = [kinfo_proc](repeating: kinfo_proc(), count: count)
			err = result!.withUnsafeMutableBufferPointer({ ( p: inout UnsafeMutableBufferPointer<kinfo_proc>) -> Int32 in
				return sysctl(namePointer, u_int(name.count), p.baseAddress, &length, nil, 0)
			})
			switch err {
			case 0:
				done = true
			case -1:
				err = errno
			case ENOMEM:
				err = 0
			default:
				fatalError()
			}
		}
	} while err == 0 && !done
	
	return result
}
