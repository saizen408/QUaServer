# Compile

**IMPORTANT** : Requires `Qt 5.7` or higher and `C++ 11`.

The source code of the `open62541` library is contained in the files:

```
src/open62541.h
src/open62541.c
```

These files are **generated** by the *CMake* build of the [original library repository](https://github.com/open62541/open62541). They were generated and copied to this repo for convenience.

```bash
cd "C:\Users\User\Desktop\Repos\open62541.git"
mkdir build; cd build
cmake -DUA_ENABLE_AMALGAMATION=ON .. -G "Visual Studio 15 2017 Win64"
```

The library version of the `open62541` files used currently in this repo is `v0.3-rc4`.

# WIP




---

## TODO

1. Find out how to use OptionSets (BitMasks).

### 7.17 OptionSetType

The OptionSetType VariableType is used to represent a bit mask. Each array element of the OptionSetValues Property contains either the human-readable representation for the corresponding bit used in the option set or an empty LocalizedText for a bit that has no specific meaning. The order of the bits of the bit mask maps to a position of the array, i.e. the first bit (least significant bit) maps to the first entry in the array, etc.

In addition to this VariableType, the DataType OptionSet can alternatively be used to represent a bit mask. As a guideline the DataType would be used when the bit mask is fixed and applies to several Variables. The VariableType would be used when the bit mask is specific for only that Variable.

The DataType of this VariableType shall be capable of representing a bit mask. It shall be either a numeric DataType representing a signed or unsigned integer, or a ByteString. For example, it can be the BitFieldMaskDataType.

The optional BitMask Property provides the bit mask in an array of Booleans. This allows subscribing to individual entries of the bit mask. The order of the bits of the bit mask points to a position of the array, i.e. the first bit points to the first entry in the array, etc.

### 12.18 BitFieldMaskDataType

This simple DataType is a subtype of UInt64 and represents a bit mask up to 32 bits where individual bits can be written without modifying the other bits.

The first 32 bits (least significant bits) of the BitFieldMaskDataType represent the bit mask and the second 32 bits represent the validity of the bits in the bit mask. When the Server returns the value to the client, the validity provides information of which bits in the bit mask have a meaning. When the client passes the value to the Server, the validity defines which bits should be written. Only those bits defined in validity are changed in the bit mask, all others stay the same. The BitFieldMaskDataType can be used as DataType in the OptionSetType VariableType.


---

# References

* <https://blog.basyskom.com/2018/want-to-give-qt-opcua-a-try/>