#pragma once

#include <cmath>

#include "summing_segment_tree.h"

struct BulkMoneyAdder {
    IncomeExpense delta = {};

    BulkMoneyAdder() = default;

    BulkMoneyAdder(double earned, double spent) {
        delta.earned = earned;
        delta.spent = spent;
    }
};

struct BulkTaxApplier {
    double ComputeFactor() const { return std::pow(factor, count); }

    int count = 0;
    double factor = 0.87;
};

class BulkLinearUpdater {
   public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkMoneyAdder& add) : add_(add) {}

    BulkLinearUpdater(const BulkTaxApplier& tax) : tax_(tax) {}

    void CombineWith(const BulkLinearUpdater& other) {
        tax_.count += other.tax_.count;
        add_.delta.earned = add_.delta.earned * other.tax_.ComputeFactor() +
                            other.add_.delta.earned;
        add_.delta.spent += other.add_.delta.spent;
    }

    IncomeExpense Collapse(IncomeExpense origin, IndexSegment segment) const {
        return {origin.earned * tax_.ComputeFactor() +
                    add_.delta.earned * static_cast<double>(segment.length()),
                origin.spent +
                    add_.delta.spent * static_cast<double>(segment.length())};
    }

   private:
    // Apply tax first, then add.
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
};