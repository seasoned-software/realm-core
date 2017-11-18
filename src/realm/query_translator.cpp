/*************************************************************************
 *
 * Copyright 2016 Realm Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 **************************************************************************/

#include <realm/query_translator.hpp>

#include <realm/query_conditions.hpp>

using namespace realm;

template <class... L>
void QueryEncoder::encode(QueryInstruction instruction, L... numbers)
{
    append_simple_instr(instruction, numbers...);
}


Query QueryTranslator::translate(char* stream, size_t size, TableRef target_table)
{
    REALM_ASSERT(target_table);
    Query q = target_table->where();

    return q;
}

std::unique_ptr<util::Buffer<char>> QueryTranslator::translate(const Query& q)
{
    q.serialize(&m_encoder);
    return nullptr;
}

template <class C>
void QueryEncoder::serialize_condition<C>()
{
    throw std::runtime_error("unsupported condition");
}


template <>
void QueryEncoder::serialize_condition<Like>()
{
    encode(QueryInstruction::instr_like);
}

template <>
void QueryEncoder::serialize_condition<Greater>()
{
    encode(QueryInstruction::instr_gt);
}
