/**
 * @file lifter_android.hpp
 * @author Farenain
 * 
 * @brief Class for lifting the code from the Android methods/classes to
 *        mjolnIR, different analysis can be applied in order to improve the
 *        representation of the IR.
 */

#ifndef LIFTER_ANDROID_HPP
#define LIFTER_ANDROID_HPP

#include <iostream>
#include <set>
// DEX
#include "DVM/dex_instructions.hpp"
#include "Analysis/dex_analysis.hpp"

// mjolnIR
#include "ir_graph.hpp"

// lifter
#include "lifter_android_instructions.hpp"

namespace KUNAI 
{
    namespace LIFTER
    {
        // Size in bits of types
        #define NIBBLE_S 4
        #define BYTE_S 8
        #define WORD_S 16
        #define DWORD_S 32
        #define ADDR_S   32
        #define QWORD_S 64
        

        class LifterAndroid
        {
        public:
            LifterAndroid();

            std::shared_ptr<MJOLNIR::IRGraph> lift_android_method(std::shared_ptr<DEX::MethodAnalysis> method_analysis);
            bool lift_android_basic_block(std::shared_ptr<DEX::DVMBasicBlock> basic_block, std::shared_ptr<MJOLNIR::IRBlock> bb);
            bool lift_android_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRBlock> bb);
        private:
            uint64_t temp_reg_id;
            uint64_t current_idx;

            std::shared_ptr<MJOLNIR::IRReg> make_android_register(std::uint32_t reg_id);
            std::shared_ptr<MJOLNIR::IRTempReg> make_temporal_register();
            std::shared_ptr<MJOLNIR::IRType> make_none_type();
            std::shared_ptr<MJOLNIR::IRConstInt> make_int(std::uint64_t value, bool is_signed, size_t type_size);
            std::shared_ptr<MJOLNIR::IRString> make_str(std::string value);
            std::shared_ptr<MJOLNIR::IRClass> make_class(DEX::Class* value);
            std::shared_ptr<MJOLNIR::IRField> make_field(DEX::FieldID* field);

            void lift_assignment_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRBlock> bb);
            void lift_arithmetic_logic_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRBlock> bb);
            void lift_ret_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRBlock> bb);
            void lift_comparison_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRBlock> bb);
            void lift_conditional_jump_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRBlock> bb);
            void lift_unconditional_jump_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRBlock> bb);
            void lift_call_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRBlock> bb);
            void lift_move_result_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRCall> call);
            void lift_load_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRBlock> bb);
            void lift_store_instruction(std::shared_ptr<DEX::Instruction> instruction, std::shared_ptr<MJOLNIR::IRBlock> bb);
            // some utilities
            void jump_target_analysis(std::vector<std::shared_ptr<KUNAI::DEX::DVMBasicBlock>> bbs);
            void fallthrough_target_analysis(std::shared_ptr<MJOLNIR::IRGraph> ir_graph);
            
            AndroidInstructions androidinstructions;
            //! It is nice that while we are lifting to annotate
            //! possible values that a register holds, in that case
            //! we annotate it into a symbolic table and if later
            //! is needed its value, we retrieve it from this variable.
            std::map<std::uint32_t, std::shared_ptr<MJOLNIR::IRType>> symbolic_table;

            //! types can be created in a map for using them when necessary
            //! in the same block same registers, same strings or same fields
            //! can be used more than once.
            std::map<std::uint32_t, std::shared_ptr<MJOLNIR::IRReg>> regs;
            std::map<std::uint32_t, std::shared_ptr<MJOLNIR::IRString>> strings;

            //! lifted blocks
            std::map<std::shared_ptr<KUNAI::DEX::DVMBasicBlock>, std::shared_ptr<MJOLNIR::IRBlock>> lifted_blocks;
        };
    }
}

#endif