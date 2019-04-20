#pragma once
#ifndef _RETXT_BASICDATASTRUCTURES_INCLUDED
#define _RETXT_BASICDATASTRUCTURES_INCLUDED

#include "globalUtilities.hpp"
#include <map>

namespace retxt
{
    template <typename containedType>
    class dataStructure
    {   
        protected:
        
        using generic = containedType;

        size_t CurrentSize;

        public:

        dataStructure() :
            CurrentSize(0)
        {
        }
        
        virtual ~dataStructure()
        {
        }
        
        virtual void clear() = 0;

        inline bool isEmpty() const
        {
            return (this->CurrentSize == 0);
        }

        inline size_t size() const
        {
            return this->CurrentSize;
        }
        
        virtual generic * toArray() const = 0;

        operator generic * () const
        {
            return this->toArray();
        }
        
        inline bool operator < ( const dataStructure<generic> &another ) const
        {
            return (this->CurrentSize < another.CurrentSize);
        }
        
        inline bool operator > ( const dataStructure<generic> &another ) const
        {
            return (this->CurrentSize > another.CurrentSize);
        }
        
        inline bool operator <= ( const dataStructure<generic> &another ) const
        {
            return (this->CurrentSize <= another.CurrentSize);
        }
        
        inline bool operator >= ( const dataStructure<generic> &another ) const
        {
            return (this->CurrentSize >= another.CurrentSize);
        }

        inline bool operator < ( const size_t size ) const
        {
            return (this->CurrentSize < size);
        }
        
        inline bool operator > ( const size_t size ) const
        {
            return (this->CurrentSize > size);
        }
        
        inline bool operator <= ( const size_t size ) const
        {
            return (this->CurrentSize <= size);
        }
        
        inline bool operator >= ( const size_t size ) const
        {
            return (this->CurrentSize >= size);
        }
    };
    
    template <typename containedType>
    class simpleSinglyLinkedStructure : 
        public dataStructure<containedType>
    {
        protected:
        
        using generic = containedType;
        
        class innerNode
        {
            public:
            
            innerNode * Next;
            generic Value;
            
            innerNode( generic value ) :
                Next(nullptr), Value(value)
            {
            }
        };
        
        innerNode * First; // Head, Top, Front
                
        public:
        
        simpleSinglyLinkedStructure () :
            dataStructure<containedType>(), 
            First(nullptr)
        {
        }

        ~simpleSinglyLinkedStructure()
        {
            this->clear();
        }

        void clear()
        {
            if (this->CurrentSize == 0) return;
            innerNode * xNode;
            for (size_t i = 0, s = this->CurrentSize; i < s; i++)
            {
                xNode = this->First->Next;
                delete this->First;
                this->First = xNode;
            }
            this->First = nullptr;
            this->CurrentSize = 0;
        }

        generic * toArray() const
        {
            if (this->CurrentSize > 0)
            {
                size_t i = 0, size = this->CurrentSize;
                generic * newArray = new generic[size];                
                const innerNode * xNode = this->First;
                while (xNode)
                {
                    newArray[i++] = xNode->Value;
                    xNode = xNode->Next;
                }
                return newArray;
            }
            else
                return nullptr;
        }

        inline bool operator == ( const simpleSinglyLinkedStructure<generic> &another ) const
        {
            return (this->First == another.First);
        }
    };
    
    template <typename containedType>
    class singlyLinkedStructure : 
        public simpleSinglyLinkedStructure<containedType>
    {
        protected:
        
        using generic = containedType;
        using innerNode = 
            typename simpleSinglyLinkedStructure<containedType>::innerNode;
        
        innerNode * Last;  // Tail, Bottom, Back
        
        static const innerNode NullNode;
        
        innerNode * node( const size_t position ) const
        {
            if (position >= this->CurrentSize) return nullptr;
            else if (position == (this->CurrentSize - 1)) return this->Last;
            innerNode * xNode = this->First;
            for (size_t i = 0; i < position; xNode = xNode->Next, i++);
            return xNode;
        }
                
        generic popOrDequeue()
        {
            innerNode * formerFirst = this->First;
            generic value = formerFirst->Value;
            this->First = formerFirst->Next;
            delete formerFirst;
            this->CurrentSize--;
            if (this->CurrentSize <= 1) this->Last = this->First;
            return value;
        }
        
        public:
        
        class safeNodeReference
        {
            friend class iterator;

            protected:

            const innerNode * InnerNode;

            public:

            safeNodeReference( const innerNode * node = nullptr ) :
                InnerNode(node)
            {
                if (node == nullptr)
                    this->InnerNode =
                        (innerNode *)(&(singlyLinkedStructure<generic>::NullNode));
            }

            inline bool exists() const
            {
                return (this->InnerNode != &(singlyLinkedStructure<generic>::NullNode));
            }

            inline generic value() const
            {
                return this->InnerNode->Value;
            }
            
            inline bool isTailOrBottom() const
            {
                return (this->InnerNode->Next == nullptr);
            }
            
            inline safeNodeReference next() const
            {
                return this->InnerNode->Next;
            }
            
            inline operator generic () const
            {
                return this->InnerNode->Value;
            }
        };
        
        singlyLinkedStructure () :
            simpleSinglyLinkedStructure<containedType>(), 
            Last(nullptr)
        {
        }

        size_t positionOf( const generic value ) const
        {
            if (this->CurrentSize == 0) return ((size_t)(-1));
            const innerNode * xNode = this->First;
            for (size_t i = 0, s = this->CurrentSize; i < s; i++)
            {
                if (xNode->Value == value) return i;
                xNode = xNode->Next;
            }
            return ((size_t)(-1));
        }
        
        inline bool contains( const generic value ) const
        {
            return (this->positionOf(value) != (size_t)(-1));
        }
        
        template <typename function>
        void forEach( function && action )
        {
            if (this->CurrentSize == 0) return;
            const innerNode * xNode = this->Top;
            for (size_t i = 0, s = this->CurrentSize; i < s; i++)
            {
                action(safeNodeReference(xNode));
                xNode = xNode->Next;
            }
        }
        
        inline generic operator [] ( const size_t position ) const
        {
            return this->node(position)->Value;
        }
    };

    template <typename containedType>
    const typename singlyLinkedStructure<containedType>::innerNode
        singlyLinkedStructure<containedType>::NullNode(containedType{});
        
    template <typename containedType>
    class simpleQueue : 
        public simpleSinglyLinkedStructure<containedType>
    {
        protected:
        
        using generic = containedType;
        using innerNode = 
            typename simpleSinglyLinkedStructure<containedType>::innerNode;
        
        innerNode * Last; // Tail, Bottom, Back
        
        public:
        
        simpleQueue () :
            simpleSinglyLinkedStructure<containedType>(),
            Last(nullptr)
        {
        }
        
        inline generic head() const
        {
            return this->First->Value;
        }
        
        inline generic tail() const
        {
            return this->Last->Value;
        }
        
        void enqueue( generic value )
        {
            innerNode * newNode = new innerNode(value), * lastNode = this->Last;
            if (this->CurrentSize == 0) this->First = this->Last = newNode;
            else 
            {
                this->Last = newNode;
                lastNode->Next = newNode;
            }
            this->CurrentSize++;
        }
        
        generic dequeue()
        {
            innerNode * formerFirst = this->First;
            generic value = formerFirst->Value;
            this->First = formerFirst->Next;
            delete formerFirst;
            this->CurrentSize--;
            if (this->CurrentSize <= 1) this->Last = this->First;
            return value;
        }
    };
    
    template <typename containedType>
    class queue : 
        public singlyLinkedStructure<containedType>
    {
        protected:
        
        using generic = containedType;
        using innerNode = 
            typename singlyLinkedStructure<containedType>::innerNode;
        using basicSafeNodeReference = 
            typename singlyLinkedStructure<containedType>::safeNodeReference;
        
        public:
        
        class safeNodeReference : public basicSafeNodeReference
        {
            public:

            safeNodeReference( const innerNode * node = nullptr ) :
                basicSafeNodeReference(node)
            {
            }
            
            inline bool isTail() const
            {
                return (this->InnerNode->Next == nullptr);
            }
        };
        
        queue () :
            singlyLinkedStructure<generic>()
        {
        }

        inline safeNodeReference head() const
        {
            return this->First;
        }
        
        inline safeNodeReference tail() const
        {
            return this->Last;
        }
        
        void enqueue( generic value )
        {
            innerNode * newNode = new innerNode(value), * lastNode = this->Last;
            if (this->CurrentSize == 0) this->First = this->Last = newNode;
            else 
            {
                this->Last = newNode;
                lastNode->Next = newNode;
            }
            this->CurrentSize++;
        }
        
        inline generic dequeue()
        {
            return this->popOrDequeue();
        }
        
        inline safeNodeReference at( const size_t position ) const
        {
            if (position >= this->CurrentSize) return nullptr;
            else return this->node(position);
        }
    };
    
    template <typename containedType>
    using safeQueueNodeReference =
        typename queue<containedType>::safeNodeReference;
   
	template <uint16_t alphabetSize, typename pointedValueType>
	class trie : public dataStructure<byte>
	{
        using generic = pointedValueType;
        
        protected:
        
        class innerNode
        {
			protected:
			
			public:
			
			generic * Value;
			innerNode Next[alphabetSize];
			
			innerNode( generic * value = nullptr ) :
				Value(value)
			{
			}
			
			inline const generic * value() const
			{
				return this->Value;
			}
			
			inline void setValue( generic * value )
			{
				this->Value = value;
			}
			
			inline void replaceValue( generic * value )
			{
				if (this->Value != nullptr) delete this->Value;
				this->Value = value;
			}
			
			inline void addNext( byte character )
			{
				this->Next[character] = new innerNode();
			}
			
			inline void addNext( byte character, generic value )
			{
				this->Next[character] = new innerNode(value);
			}
			
			inline innerNode * operator [] ( byte character )
			{
				if (this->Next.count(character) == 0) return nullptr;
				else return this->Next[character];
			}

            inline operator generic () const
            {
                return (*(this->Value));
            }
		};
		
		innerNode * Root;
		size_t EntriesCount;
		
		const generic * value( const byte * const entry ) const
		{
			innerNode * current = this->Root, * bestCandidate = this->Root;			
			for (size_t i = 0; entry[i] != 0; i++)
			{
				if ((*current)[entry[i]] != nullptr)
				{
					current = (*current)[entry[i]];
					if (current->value() != nullptr) bestCandidate = current;
				}
				else break;
			}
			return bestCandidate->value();
		}
		
		const generic * value( const byte * entry, size_t entrySize ) const
		{
			innerNode * current = this->Root, * bestCandidate = this->Root;			
			for (size_t i = 0; i < entrySize; i++)
			{
				if ((*current)[entry[i]] != nullptr)
				{
					current = (*current)[entry[i]];
					if (current->value() != nullptr) bestCandidate = current;
				}
				else break;
			}
			return bestCandidate->value();
		}
		
        template<typename function>
        void valuesWalk( innerNode * currentRoot, function &&action )
        {
			if (currentRoot->value() != nullptr)
				action(currentRoot->value());
			for (auto next : currentRoot->Next)
				trieWalk(next.second, action);
        }
		
        template<typename function>
        void entriesWalk( innerNode * currentRoot, function &&action ) const
        {
			if (currentRoot->value() != nullptr)
				action(currentRoot->Value);
			for (auto next : currentRoot->Next)
				entriesWalk(next.second, action);
        }
		
		public:
		
		trie() :
			dataStructure<const byte *>(), Root(nullptr), EntriesCount(0)
		{
		}
	
		inline uint16_t entriesCount() const
		{
			return this->EntriesCount;
		}
		
		void clear()
		{
			delete this->Root;
		}
		
		void insert( const byte * entry, size_t entrySize, generic value )
		{
			innerNode * current = this->Root, * bestCandidate = this->Root;
			size_t i;
			for (i = 0; i < entrySize; i++)
			{
				if ((*current)[entry[i]] != nullptr)
				{
					current = (*current)[entry[i]];
					if (current->value() != nullptr) bestCandidate = current;
				}
				else break;
			}
			for (; entry[i] != 0; i++, this->CurrentSize++) // for (; i < entrySize; i++, this->CurrentSize++)
			{
				bestCandidate->addNext(entry[i]);
				bestCandidate = (*bestCandidate)[entry[i]];
			}
			if (bestCandidate->value() == nullptr) 
			{
				this->EntriesCount++;
				bestCandidate->setValue(&value); // bestCandidate->setValue(value);
			}
		}
		
		void replace( const byte * entry, generic newValue );
		
		void replace( const byte * entry, size_t entrySize, generic newValue );
		
		inline generic & retrieve( const byte * entry )
		{
			return (*(this->value(entry)));
		}
		
		inline generic & retrieve( const byte * entry, size_t entrySize )
		{
			return (*(this->value(entry, entrySize)));
		}

		inline bool contains( const byte * entry ) const
		{
			return (this->value(entry) != nullptr);
		}
		
		inline bool contains( const byte * entry, size_t entrySize ) const
		{
			return (this->value(entry, entrySize) != nullptr);
		}
		
        template<typename function>
		inline void forEachEntry( function && action ) const
		{
			this->entriesWalk(this->Root, action);
		}
		
        template<typename function>
		void forEachValue( function &&action )
		{
			
		}

		const byte * * toArray() const
		{
            if (this->CurrentSize > 0)
            {
                size_t i = 0, size = this->CurrentSize;
                byte * * newArray = new byte*[size];

                this->forEachEntry(
                    [&]( byte * value )
                    {
						newArray[i++] = value;
					});

                return ((const byte * *)(newArray));
            }
            else
                return nullptr;
		}
	};
}

#endif // _RETXT_BASICDATASTRUCTURES_INCLUDED
