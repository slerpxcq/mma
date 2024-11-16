#pragma once

#define MM_NONCOPYABLE(_class_) \
	_class_(const _class_&) = delete; \
	_class_(_class_&&) noexcept = default; \
	_class_& operator=(const _class_&) = delete; \
	_class_& operator=(_class_&&) = default;

#define MM_NONMOVABLE(_class_) \
	_class_(const _class_&) = delete; \
	_class_(_class_&&) noexcept = delete; \
	_class_& operator=(const _class_&) = delete; \
	_class_& operator=(_class_&&) = delete;

