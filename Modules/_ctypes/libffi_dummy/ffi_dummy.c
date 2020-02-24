
#include <ffi.h>


int can_return_struct_as_int(size_t s)
{
	return s == 1 || s == 2 || s == 4;
}

int can_return_struct_as_sint64(size_t s)
{
	return s == 8;
}

ffi_status ffi_prep_cif_machdep(ffi_cif* cif)
{
	/* Set the return type flag */
	switch (cif->rtype->type)
	{
	case FFI_TYPE_VOID:
	case FFI_TYPE_SINT64:
	case FFI_TYPE_FLOAT:
	case FFI_TYPE_DOUBLE:
	case FFI_TYPE_LONGDOUBLE:
		cif->flags = (unsigned)cif->rtype->type;
		break;

	case FFI_TYPE_STRUCT:
		/* MSVC returns small structures in registers.  Put in cif->flags
		   the value FFI_TYPE_STRUCT only if the structure is big enough;
		   otherwise, put the 4- or 8-bytes integer type. */
		if (can_return_struct_as_int(cif->rtype->size))
			cif->flags = FFI_TYPE_INT;
		else if (can_return_struct_as_sint64(cif->rtype->size))
			cif->flags = FFI_TYPE_SINT64;
		else
			cif->flags = FFI_TYPE_STRUCT;
		break;

	case FFI_TYPE_UINT64:
#ifdef _WIN64
	case FFI_TYPE_POINTER:
#endif
		cif->flags = FFI_TYPE_SINT64;
		break;

	default:
		cif->flags = FFI_TYPE_INT;
		break;
	}

	return FFI_OK;
}

int
ffi_call(/*@dependent@*/ ffi_cif* cif,
	void (*fn)(),
	/*@out@*/ void* rvalue,
	/*@dependent@*/ void** avalue)
{
	return -1; /* theller: Hrm. */
}


ffi_status
ffi_prep_closure_loc(ffi_closure* closure,
	ffi_cif* cif,
	void (*fun)(ffi_cif*, void*, void**, void*),
	void* user_data,
	void* codeloc)
{
	return FFI_OK;
}

ffi_status ffi_prep_cif(/*@out@*/ /*@partial@*/ ffi_cif* cif,
	ffi_abi abi, unsigned int nargs,
	/*@dependent@*/ /*@out@*/ /*@partial@*/ ffi_type* rtype,
	/*@dependent@*/ ffi_type** atypes)
{
	return FFI_BAD_TYPEDEF;
}
