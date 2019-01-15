cmake_minimum_required(VERSION 3.1)

set(NAME KhSTL)

set(ROOT_HEADERS
 KhAlgorithm.h
   KhArray.h
   KhDeque.h
   KhHashMap.h
   KhHashSet.h
   KhHeap.h
   KhList.h
   KhMap.h
   KhPair.h
   KhQueue.h
   KhSet.h
   KhSingleton.h
   KhStack.h
   KhSTL.h
   KhString.h
   KhVector.h
   KhAlgorithm
    KhArray
    KhDeque
    KhHashMap
    KhHashSet
    KhHeap
    KhList
    KhMap
    KhQueue
    KhSet
    KhSingleton
    KhStack
    KhSTL
    KhString
    KhVector
)

set(UTILITY_HEADERS
   Utility/TypeDef.h
   Utility/TypeHash.h
   Utility/TypeHashBase.h
   Utility/TypeHashIterator.h
   Utility/TypeHashValue.h
   Utility/TypeIterator.h
   Utility/TypePair.h
   Utility/TypeRBTree.h
   Utility/TypeRBTreeAlloc.h
   Utility/TypeRBTreeIterator.h
   Utility/TypeReverseIterator.h
   Utility/TypeSingleton.h
   Utility/TypeTreeNode.h
   )
   

set(ALGORITHM_HEADERS
   Algorithm/TypeAlgorithm.h
   Allocator/TypeAllocator.h
   Allocator/TypeConstruct.h
   Allocator/TypeMemory.h
   )


set(CONTAINER_HEADERS
	Container/Array/TypeArray.h
   Container/Array/TypeArrayIterator.h
   Container/Deque/TypeDeque.h
   Container/Deque/TypeDequeAllocator.h
   Container/Deque/TypeDequeIterator.h
   Container/Deque/TypeDequeValue.h
   Container/Heap/TypeHeap.h
   Container/List/TypeLinkedList.h
   Container/List/TypeList.h
   Container/List/TypeListAllocator.h
   Container/List/TypeListIterator.h
   Container/List/TypeListValue.h
   Container/Map/TypeMap.h
   Container/Map/TypeMapTraits.h
   Container/Map/TypeMultiMap.h
   Container/Map/TypeUnorderedMap.h
   Container/Map/TypeUnorderedMultiMap.h
   Container/Queue/TypeQueue.h
   Container/Set/TypeMultiSet.h
   Container/Set/TypeSet.h
   Container/Set/TypeSetTraits.h
   Container/Set/TypeUnorderedMultiSet.h
   Container/Set/TypeUnorderedSet.h
   Container/Stack/TypeStack.h
   Container/String/TypeString.h
   Container/Vector/TypeVector.h
   Container/Vector/TypeVectorAllocator.h
   Container/Vector/TypeVectorValue.h
)
set(CONTAINER_SCRS Container/String/TypeString.cpp )
 
source_group("Utility" FILES ${UTILITY_HEADERS})
source_group("Algorithm" FILES ${ALGORITHM_HEADERS})
source_group("Container" FILES ${CONTAINER_HEADERS})
source_group("Container" FILES ${CONTAINER_SCRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	
add_library(${NAME} ${ROOT_HEADERS} 
${UTILITY_HEADERS} 
${ALGORITHM_HEADERS} 
${CONTAINER_HEADERS} 
${CONTAINER_SCRS}
)

set_target_properties(${NAME} PROPERTIES FOLDER "ThirdParty") 
set_target_properties(${NAME} PROPERTIES LINKER_LANGUAGE cxx) 
